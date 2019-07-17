#include "UDPManager.h"

using Utils::print;
using Utils::LogLevel;

int UDPManager::send_socketfd = 0;
int UDPManager::recv_socketfd = 0;
std::atomic<bool> UDPManager::running(false);
UDPManager::Connection_Status UDPManager::connection_status = UDPManager::Connection_Status::NOT_YET_CONNECTED;
struct addrinfo UDPManager::hints;
struct addrinfo * UDPManager::sendinfo = NULL;
struct addrinfo * UDPManager::recvinfo = NULL;
Event UDPManager::setup;
std::mutex UDPManager::mutex;

bool UDPManager::start_udp(const char * hostname, const char * send_port, const char * recv_port) {
  int rv;
  int enable = 1;
  std::lock_guard<std::mutex> guard(mutex);

  /////////////////////
  // SETUP UDP SEND PORT
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  
  // Get destination address
  if ((rv = getaddrinfo(hostname, send_port, &hints, &sendinfo)) != 0) {
    print(LogLevel::LOG_ERROR, "UDP send Error get addrinfo: %s\n", gai_strerror(rv));
    goto RETURN_ERROR;
  }

  // Create SEND socket
  if ((send_socketfd = socket(sendinfo->ai_family, sendinfo->ai_socktype, sendinfo->ai_protocol)) == -1) {
    print(LogLevel::LOG_ERROR, "UDP send Error getting send socket\n");
    goto RETURN_ERROR;
  }

  // Allow port reuse
  if (setsockopt(send_socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    print(LogLevel::LOG_ERROR, "UDP SEND setsockopt(SO_REUSEADDR) failed: %s\n", strerror(errno));
    goto RETURN_ERROR;
  }

  /////////////////////
  // SETUP UDP RECV PORT
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  
  // For recv address
  if ((rv = getaddrinfo(NULL, recv_port, &hints, &recvinfo)) != 0) {
    print(LogLevel::LOG_ERROR, "UDP Error getting addrinfo: %s\n", gai_strerror(rv));
    goto RETURN_ERROR;
  }

  // Create recv socket
  if ((recv_socketfd = socket(recvinfo->ai_family, recvinfo->ai_socktype, recvinfo->ai_protocol)) == -1) {
    print(LogLevel::LOG_ERROR, "UDP send Error getting send socket\n");
    goto RETURN_ERROR;
  }

  // Allow port reuse
  if (setsockopt(recv_socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    print(LogLevel::LOG_ERROR, "UDP RECV setsockopt(SO_REUSEADDR) failed: %s\n", strerror(errno));
    goto RETURN_ERROR;
  }

  // Bind to port (tells the OS the default incoming address)
  if (bind(recv_socketfd, recvinfo->ai_addr, recvinfo->ai_addrlen) != 0) {
    print(LogLevel::LOG_ERROR, "UDP bind failed: %s\n", strerror(errno));
    goto RETURN_ERROR;
  }

  // Connect (tells the OS the default destination)
  // Connect to the send address
  // if(connect(socketfd, sendinfo->ai_addr, sendinfo->ai_addrlen) == -1) {
  //   print(LogLevel::LOG_ERROR, "UDP connecting failed: %s\n", strerror(errno));
  //   goto RETURN_ERROR;
  // }

  // freeaddrinfo(sendinfo);
  freeaddrinfo(recvinfo);
  return true;

RETURN_ERROR:
  print(LogLevel::LOG_ERROR, "UDP returning error on startup");
  close(send_socketfd);
  close(recv_socketfd);
  freeaddrinfo(sendinfo);
  freeaddrinfo(recvinfo);
  return false;
}

int UDPManager::udp_recv(uint8_t* recv_buf, uint8_t len) {
  socklen_t fromlen;                 // needed for recvfrom
  struct sockaddr_storage fromaddr;  // needed for recvfrom
  fromlen = sizeof fromaddr;         // needed for recvfrom

  int byte_count = recvfrom(recv_socketfd, recv_buf, len, 0, 
      (struct sockaddr *)&fromaddr, &fromlen);
  if (byte_count == -1) {
    print(LogLevel::LOG_ERROR, "UDP recv failed: %s\n", strerror(errno));
    // TODO Determine if this is an error worth reporting
    return 0;
  }
  recv_buf[byte_count] = '\0';
  // print(LogLevel::LOG_DEBUG, "UDP recv %d bytes, they are: %s \n", byte_count, recv_buf);
  
  return byte_count;
}

bool UDPManager::udp_parse(uint8_t* buf, uint8_t len) {
  if (buf[0] == 'P') {    // for an example, lets send the first byte to be P, for PING 
    return true;          // if we get ping, we know it's a dummy
  } else if (buf[0] == 13) {
    //send command to transition to abort/safe mode here  
    Command::put(Command::TRANS_ABORT,0);
    return true;
  } else {
    Command::set_error_flag(Command::Network_Command_ID::SET_NETWORK_ERROR,NETWORKErrors::UDP_E_BRAKE_ERROR);
    print(LogLevel::LOG_INFO, "Parsed %d bytes, they are: %s\n", len, buf);   }
  return false;
}

int UDPManager::udp_send(uint8_t* buf, uint8_t len) { 
  int byte_count = sendto(send_socketfd, buf, len, 0,
      sendinfo->ai_addr, sendinfo->ai_addrlen);
  if (byte_count == -1) {
    print(LogLevel::LOG_ERROR, "UDP recv failed: %s\n", strerror(errno));
    // TODO Determine if this is an error worth reporting
    return 0;
  }
  return byte_count;
}

void UDPManager::connection_monitor(const char * hostname, const char * send_port, const char * recv_port) {
  setup.reset();

  // Create UDP socket
  if (!start_udp(hostname, send_port, recv_port)) {
    print(LogLevel::LOG_ERROR, "Error setting up UDP\n");
    return; 
  }

  // Setup variables for UDP loop
  bool is_connected = false;
  int rv;
  int byte_count;
  struct pollfd fds[1];
  fds[0].fd = recv_socketfd;
  fds[0].events = POLLIN;
  uint8_t send_buffer[] = {'A', 'C', 'K'};
  uint8_t read_buffer[8];

  /*
   * Timeout = -min(p) - min(D1) + heartbeat_period + max(D1) + max(p) 
   * p is processing time
   * delta: the time to receive the python's ping
   * heartbeat_period is the period at which this happens
   */
  int heartbeat_period  = 0;  // milliseconds. Period that Python sends pings 
  int max_delta         = 0;  // milliseconds. Max one way trip time, Python --to-> Pod
  int min_delta         = 0;  // milliseconds. Min one way trip time, Python --to-> Pod
  int max_p             = 0;  // milliseconds. Max processing time on Pod
  int min_p             = 0;  // milliseconds. Min processing time on Pod
  // Grab all configuration variables
  if (!(ConfiguratorManager::config.getValue("udp_heartbeat_period", heartbeat_period) && 
      ConfiguratorManager::config.getValue("udp_d1_max", max_delta) &&
      ConfiguratorManager::config.getValue("udp_d1_min", min_delta) &&
      ConfiguratorManager::config.getValue("udp_p_max", max_p) &&
      ConfiguratorManager::config.getValue("udp_p_min", min_p))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR -UDP- Missing necessary configuration\n");
    exit(1);  // Crash hard on this error
  }
  int connected_timeout = heartbeat_period + max_delta + max_p - min_p - min_delta;
  int timeout = -1; 
  
  running.store(true);
  print(LogLevel::LOG_INFO, "UDP Setup complete\n");
  setup.invoke();

  // TODO: @Evan Remove this
  // Send ack just to test
  udp_send(send_buffer, sizeof(send_buffer));  

  // Poll indefinitely until a ping is received, then go into ping-ack loop.
  while (running) {
    rv = poll(fds, 1, timeout);  // http://beej.us/guide/bgnet/html/single/bgnet.html#indexId434909-276
    if (rv == -1) {  // ERROR occured in poll()
      print(LogLevel::LOG_ERROR, "UDP poll() failed: %s\n", strerror(errno));
      Command::put(Command::SET_NETWORK_ERROR, NETWORKErrors::UDP_DISCONNECT_ERROR);
      is_connected = false;
    } else if (rv == 0) {  // Timeout occured 
      print(LogLevel::LOG_ERROR, "UDP timeout\n");
      Command::put(Command::SET_NETWORK_ERROR, NETWORKErrors::UDP_DISCONNECT_ERROR);
      is_connected = false;
    } else {
      if (fds[0].revents & POLLIN) {  // There is data to be read from UDP
        timeout = connected_timeout;  // Set timeout to appropriate value
        byte_count = udp_recv(read_buffer, sizeof(read_buffer));  // Read message
        if (byte_count > 0 && udp_parse(read_buffer, byte_count)) {  // Check if PING. Returns true if message was PING
          byte_count = udp_send(send_buffer, sizeof(send_buffer));  // Respond with ACK
          // print(LogLevel::LOG_DEBUG, "sent %d bytes, \n", byte_count); 
          if (!is_connected) {
            print(LogLevel::LOG_INFO, "UDP Connected! \n");
            Command::put(Command::CLR_NETWORK_ERROR, NETWORKErrors::UDP_DISCONNECT_ERROR);
            is_connected = true;
          }
        }
      } else {
        // print(LogLevel::LOG_ERROR, "UDP poll event, but not on specified socket with specified event\n");
        // TODO: Once Unified Command Queue is implemented, consider this as a failure & write to queue
      }
    }
  }

  freeaddrinfo(sendinfo);  // Free memory
  close(send_socketfd);  // Close socket
  close(recv_socketfd);  // Close socket
  setup.reset();  // Reset event (important when tests are run repeatedly)
  print(LogLevel::LOG_INFO, "UDP Exiting\n");
}

void UDPManager::close_client() {
  std::lock_guard<std::mutex> guard(mutex);
  running.store(false);
  shutdown(recv_socketfd, SHUT_RDWR);
  shutdown(send_socketfd, SHUT_RDWR);
}
