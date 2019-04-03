#include "UDPManager.h"

using Utils::print;
using Utils::LogLevel;

int UDPManager::socketfd = 0;
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

  // Create socket
  if ((socketfd = socket(sendinfo->ai_family, sendinfo->ai_socktype, sendinfo->ai_protocol)) == -1) {
    print(LogLevel::LOG_ERROR, "UDP send Error getting socket\n");
    goto RETURN_ERROR;
  }

  /////////////////////
  // SETUP UDP RECV PORT
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;
  
  // For recv address
  if ((rv = getaddrinfo(hostname, recv_port, &hints, &recvinfo)) != 0) {
    print(LogLevel::LOG_ERROR, "UDP Error getting addrinfo: %s\n", gai_strerror(rv));
    goto RETURN_ERROR;
  }

  // Allow port reuse
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    print(LogLevel::LOG_ERROR, "UDP setsockopt(SO_REUSEADDR) failed: %s\n", strerror(errno));
    goto RETURN_ERROR;
  }

  // Bind to port (tells the OS the default incoming address)
  if (bind(socketfd, recvinfo->ai_addr, recvinfo->ai_addrlen) != 0) {
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
  close(socketfd);
  freeaddrinfo(sendinfo);
  freeaddrinfo(recvinfo);
  return false;
}

int UDPManager::udp_recv(uint8_t* recv_buf, uint8_t len) {
  socklen_t fromlen;                 // needed for recvfrom
  struct sockaddr_storage fromaddr;  // needed for recvfrom
  fromlen = sizeof fromaddr;         // needed for recvfrom

  int byte_count = recvfrom(socketfd, recv_buf, len, 0, 
      (struct sockaddr *)&fromaddr, &fromlen);
  if (byte_count == -1) {
    print(LogLevel::LOG_ERROR, "UDP recv failed: %s\n", strerror(errno));
    // TODO Determine if this is an error worth reporting
    return 0;
  }
  recv_buf[byte_count] = '\0';
  print(LogLevel::LOG_DEBUG, "UDP recv %d bytes, they are: %s \n", byte_count, recv_buf);
  
  return byte_count;
}

bool UDPManager::udp_parse(uint8_t* buf, uint8_t len) {
  if (buf[0] == 'P') {    // for an example, lets send the first byte to be P, for PING 
    return true;          // if we get ping, we know it's a dummy
  } else {
    // Print whatever we got because it wasn't PING
    print(LogLevel::LOG_INFO, "Parsed %d bytes, they are: %s\n", len, buf);   }
  return false;
}

int UDPManager::udp_send(uint8_t* buf, uint8_t len) { 
  int byte_count = sendto(socketfd, buf, len, 0,
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
  int rv;
  int byte_count;
  struct pollfd fds[1];
  fds[0].fd = socketfd;
  fds[0].events = POLLIN;
  uint8_t send_buffer[] = {'A', 'C', 'K'};
  uint8_t read_buffer[8];

  /*
   * Timeout = -min(p) - min(D1) + T + max(D1) + max(p) 
   * p is processing time
   * D1 is delta 1: the time to receive the python's ping
   * T is the period at which this happens
   */
  // TODO: Read in the following values from Config file, once that branch is merged into master
  int T      = 1000;  // milliseconds. Period that Python sends pings 
  int D1_max = 10;    // milliseconds. Max one way trip time, Python --to-> Pod
  int D1_min = 2;     // milliseconds. Min one way trip time, Python --to-> Pod
  int P_max  = 5;     // milliseconds. Max processing time on Pod
  int P_min  = 1;     // milliseconds. Min processing time on Pod
  int connected_timeout = T + D1_max + P_max - P_min - D1_min;
  int timeout = -1; 
  
  running.store(true);
  print(LogLevel::LOG_INFO, "UDP Setup complete\n");
  setup.invoke();
  // Poll indefinitely until a ping is received, then go into ping-ack loop.
  while (running) {
    rv = poll(fds, 1, timeout);  // http://beej.us/guide/bgnet/html/single/bgnet.html#indexId434909-276
    if (rv == -1) {  // ERROR occured in poll()
      print(LogLevel::LOG_ERROR, "UDP poll() failed: %s\n", strerror(errno));
      // TODO: Once Unified Command Queue is implemented, consider this as a failure & write to queue
    } else if (rv == 0) {  // Timeout occured 
      print(LogLevel::LOG_ERROR, "UDP timeout\n");
      // TODO: Once Unified Command Queue is implemented, consider this as a failure & write to queue
    } else {
      if (fds[0].revents & POLLIN) {  // There is data to be read from UDP
        timeout = connected_timeout;  // Set timeout to appropriate value
        byte_count = udp_recv(read_buffer, sizeof(read_buffer));  // Read message
        if (byte_count > 0 && udp_parse(read_buffer, byte_count)) {  // Check if PING. Returns true if message was PING
          byte_count = udp_send(send_buffer, sizeof(send_buffer));  // Respond with ACK
          print(LogLevel::LOG_DEBUG, "sent %d bytes, \n", byte_count); 
        }
      } else {
        // print(LogLevel::LOG_ERROR, "UDP poll event, but not on specified socket with specified event\n");
        // TODO: Once Unified Command Queue is implemented, consider this as a failure & write to queue
      }
    }
  }

  freeaddrinfo(sendinfo);  // Free memory
  close(socketfd);  // Close socket
  setup.reset();  // Reset event (important when tests are run repeatedly)
  print(LogLevel::LOG_INFO, "UDP Exiting\n");
}

void UDPManager::close_client() {
  std::lock_guard<std::mutex> guard(mutex);
  running.store(false);
  shutdown(socketfd, SHUT_RDWR);
}
