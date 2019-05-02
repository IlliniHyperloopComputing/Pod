#include "TCPManager.h"
#include "Command.h"

using std::vector;
using std::thread;
using Utils::print;
using Utils::LogLevel;
using Utils::write_all_to_socket;
using std::shared_ptr;

int TCPManager::socketfd = 0;
Event TCPManager::connected;
Event TCPManager::closing;

std::atomic<bool> TCPManager::running(false);
std::mutex TCPManager::setup_shutdown_mutex;

TCPManager::TCPSendIDs TCPManager::TCPID;

shared_ptr<UnifiedState> TCPManager::data_to_send;  
shared_ptr<UnifiedState> TCPManager::local_to_send;
std::mutex TCPManager::data_mutex;  
int64_t TCPManager::write_loop_timeout;

int64_t TCPManager::stagger_times[3];   // For sendig data to the TCP Write loop
int64_t TCPManager::last_sent_times[3];   // For sending data to the TCP Write loop

int TCPManager::connect_to_server(const char * hostname, const char * port) {
  std::lock_guard<std::mutex> guard(setup_shutdown_mutex);  // Used to protect socketfd (TSan datarace)
  struct addrinfo hints, *servinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int rv;
  if ((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "TCP Error get addrinfo\n");
    return false;
  }

  if ((socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "TCP Error getting socket\n");
    return false;
  }

  if (connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    close(socketfd);
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "TCP Error connecting\n");
    return false;
  }

  freeaddrinfo(servinfo);
  return socketfd;
}

int TCPManager::write_data() {
  int64_t cur_time = Utils::microseconds();

  // Determine if anything is going to want to send data
  bool do_update = false;
  for (int i = 0; i < 3 && !do_update; i++) {
    do_update |= (cur_time - last_sent_times[i] > stagger_times[i]);
  }

  if (!do_update) {
    return 1;  // Return early, since we are not sending data
  }
  
  // There must be an update, copy in data
  data_mutex.lock();  // Protect access to TCPManger::data_to_send
  local_to_send = data_to_send;
  data_mutex.unlock();
  //  This is the first time threshold
  if (cur_time - last_sent_times[0] > stagger_times[0]) {
    last_sent_times[0] = cur_time;
    if ((write_all_to_socket(socketfd, &TCPID.motion_id, sizeof(uint8_t))) ||
        (write_all_to_socket(socketfd, reinterpret_cast<uint8_t *>(local_to_send->motion_data.get()), sizeof(MotionData)) <= 0) ||  // NOLINT
        (write_all_to_socket(socketfd, &TCPID.error_id, sizeof(uint8_t))) ||
        (write_all_to_socket(socketfd, reinterpret_cast<uint8_t*>(local_to_send->errors.get()), sizeof(Errors)) <= 0) ||  // NOLINT  
        (write_all_to_socket(socketfd, &TCPID.state_id, sizeof(uint8_t))) ||
        (write_all_to_socket(socketfd, reinterpret_cast<uint8_t*>(&local_to_send->state), sizeof(uint32_t)) <= 0)) {  // NOLINT
      return -1;
    }
  }
  //  This is the second time threshold 
  if (cur_time - last_sent_times[1] > stagger_times[1]) {  
    last_sent_times[1] = cur_time;
    if ((write_all_to_socket(socketfd, &TCPID.can_id, sizeof(uint8_t))) ||
        (write_all_to_socket(socketfd, reinterpret_cast<uint8_t*>(local_to_send->can_data.get()), sizeof(MotionData)) <= 0)) {  //NOLINT
      return -1;
    }
  }
  //  This is the third time threshold 
  if (cur_time - last_sent_times[2] > stagger_times[2]) {  
    last_sent_times[2] = cur_time;
    if ((write_all_to_socket(socketfd, &TCPID.pru_id, sizeof(uint8_t))) ||
        (write_all_to_socket(socketfd, reinterpret_cast<uint8_t*>(local_to_send->pru_data.get()), sizeof(PRUData)) <= 0) ||  //NOLINT
        (write_all_to_socket(socketfd, &TCPID.i2c_id, sizeof(uint8_t))) ||
        (write_all_to_socket(socketfd, reinterpret_cast<uint8_t*>(local_to_send->i2c_data.get()), sizeof(I2CData)) <= 0) ||  //NOLINT
        (write_all_to_socket(socketfd, &TCPID.adc_id, sizeof(uint8_t))) ||
        (write_all_to_socket(socketfd, reinterpret_cast<uint8_t*>(local_to_send->adc_data.get()), sizeof(ADCData))<= 0)) {  //NOLINT
      return -1;
    }
  }  
  return 1;  // Return success
}

void TCPManager::write_loop() {
  bool active_connection = true;
  while (running && active_connection) {
    closing.wait_for(write_loop_timeout);
    int written = write_data();
    // print(LogLevel::LOG_DEBUG, "TCP Wrote %d bytes\n", written);
    active_connection = written != -1;
  }
  print(LogLevel::LOG_INFO, "TCP write Loop exiting.\n");
}

int TCPManager::read_command(uint32_t * id, uint32_t * com) {
  uint8_t bytes[sizeof(Command::Network_Command)];
  int bytes_read = read(socketfd, bytes, sizeof(bytes));
  *id =  *(reinterpret_cast<uint32_t*>(bytes));
  *com = *(reinterpret_cast<uint32_t*>((bytes+4)));
  return bytes_read;
}

void TCPManager::read_loop() {
  bool active_connection = true;
  uint32_t ID;
  uint32_t Command;
  while (running && active_connection) {
    int bytes_read = read_command(&ID, &Command);
    active_connection = bytes_read > 0;
    if (bytes_read > 0) {
      Command::put(ID, Command);
      // print(LogLevel::LOG_EDEBUG, "Bytes read: %d Read command %d %d\n", bytes_read, ID, Command);
    }
  }
  print(LogLevel::LOG_INFO, "TCP read Loop exiting.\n");
}

void TCPManager::tcp_loop(const char * hostname, const char * port) {
  connected.reset();
  closing.reset();
  running.store(true);

  if (!( ConfiguratorManager::config.getValue("tcp_write_loop_timeout", write_loop_timeout) &&
      ConfiguratorManager::config.getValue("tcp_stagger_time1", stagger_times[0]) &&
      ConfiguratorManager::config.getValue("tcp_stagger_time2", stagger_times[1]) &&
      ConfiguratorManager::config.getValue("tcp_stagger_time3", stagger_times[2]))) {
    print(LogLevel::LOG_ERROR, "TCP CONFIG FILE ERROR: Missing necessary configuration\n");
    exit(1);  // Crash hard on this error
  }
  last_sent_times[0] = -1000000;  // Initialize these times to a large negative number, so sending happens right away
  last_sent_times[1] = -1000000;
  last_sent_times[2] = -1000000;
  
  while (running) {
    int fd = connect_to_server(hostname, port);
    if (fd > 0) {
      print(LogLevel::LOG_INFO, "TCP Starting network threads\n");
      thread read_thread(read_loop);
      thread write_thread(write_loop);

      connected.invoke();  // Threads started, show simulator we are connected

      read_thread.join();
      write_thread.join();

      print(LogLevel::LOG_INFO, "TCP Connection lost\n");

    } else {
      running.store(false);
      break;
    }
  }   

  close(socketfd);  // At last, close the socket
  connected.reset();  
  print(LogLevel::LOG_INFO, "TCP Exiting loop\n");
}

void TCPManager::close_client() {
  std::lock_guard<std::mutex> guard(setup_shutdown_mutex);  // Used to protect socketfd (TSan datarace)
  running.store(false);  // Will cause the tcp_loop to exit once threads join.
  closing.invoke();  // write_thread sleeps using an event. Invoke the event to stop further sleeping
  shutdown(socketfd, SHUT_RDWR);  // Causes read(socketfd) or write(socketfd) to return. 
}




