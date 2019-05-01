#include "TCPManager.h"
#include "Command.h"

using std::vector;
using std::thread;
using Utils::print;
using Utils::LogLevel;
using std::shared_ptr;

int TCPManager::socketfd = 0;
Event TCPManager::connected;
Event TCPManager::closing;

std::atomic<bool> TCPManager::running(false);
std::mutex TCPManager::mutex;

// I'm not sure how to get the unified state to the TCPManager
SafeQueue<shared_ptr<UnifiedState>> TCPManager::write_queue;  

int TCPManager::connect_to_server(const char * hostname, const char * port) {
  std::lock_guard<std::mutex> guard(mutex);  // Used to protect socketfd (TSan datarace)
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

int TCPManager::read_command(uint32_t * ID, uint32_t * Command) {
  uint8_t bytes[sizeof(Command::Network_Command)];
  int bytes_read = read(socketfd, bytes, sizeof(bytes));
  *ID =  *((uint32_t *)bytes);
  *Command = *((uint32_t *) (bytes+4));
  return bytes_read;
}

int TCPManager::write_data() {
  vector<int32_t> vals;
  static vector<int64_t> last_sent_time;
  int64_t start_time = Utils::microseconds();
  for(int i = 0; i < 3; i++) last_sent_time.push_back(start_time);  
  if(start_time - last_sent_time[0] > stagger_times[0]){  //  This is the first time threshold
    vals.push_back(Command::POD_STATE);
    vals.push_back(Command::POSITION); 
    vals.push_back(Command::VELOCITY);
    vals.push_back(Command::ACCELERATION);
    last_sent_time[0] = start_time;
  }
  if(start_time - last_sent_time[1] > stagger_times[1]){  //  This is the second time threshold 
    vals.push_back(Command::TEMPERATURE);
    last_sent_time[1] = start_time;
  }
  if(start_time - last_sent_time[2] > stagger_times[2]){  //  This is the third time threshold 
    vals.push_back(Command::BRAKE_STATUS);
    vals.push_back(Command::MOTOR_STATUS);
    last_sent_time[2] = start_time;
  }  
  return write(socketfd, vals.data(), vals.size() * sizeof(int32_t));
}

void TCPManager::read_loop() {
  bool active_connection = true;
  uint32_t ID;
  uint32_t Command;
  while (running && active_connection) {
    int bytes_read = read_command(&ID, &Command);
    active_connection = bytes_read > 0;
    if (bytes_read > 0) {
      print(LogLevel::LOG_EDEBUG, "Bytes read: %d Read command %d %d\n", bytes_read, ID, Command);
      Command::put(ID, Command);
    }
  }
  print(LogLevel::LOG_INFO, "TCP read Loop exiting.\n");
}

void TCPManager::write_loop() {
  bool active_connection = true;
  while (running && active_connection) {
    closing.wait_for(1000000);
    int written = write_data();
    print(LogLevel::LOG_DEBUG, "TCP Wrote %d bytes\n", written);
    active_connection = written != -1;
  }
  print(LogLevel::LOG_INFO, "TCP write Loop exiting.\n");
}

void TCPManager::tcp_loop(const char * hostname, const char * port) {
  connected.reset();
  closing.reset();
  running.store(true);
  
  if (!(ConfiguratorManager::config.getValue("tcp_stagger_time1", stagger_times[0]) &&
       ConfiguratorManager::config.getValue("tcp_stagger_time2", stagger_times[0]) &&
       ConfiguratorManager::config.getValue("tcp_stagger_time3", stagger_times[2]))){
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: Missing necessary configuration");
    running.store(false);
  }

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
  std::lock_guard<std::mutex> guard(mutex);  // Used to protect socketfd (TSan datarace)
  running.store(false);  // Will cause the tcp_loop to exit once threads join.
  closing.invoke();  // write_thread sleeps using an event. Invoke the event to stop further sleeping
  shutdown(socketfd, SHUT_RDWR);  // Causes read(socketfd) or write(socketfd) to return. 
}




