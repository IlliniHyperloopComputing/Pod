#include "TCPManager.h"

using std::vector;
using std::make_shared;
using std::shared_ptr;
using std::thread;
using Utils::print;
using Utils::LogLevel;

int TCPManager::socketfd = 0;
Event TCPManager::connected;
Event TCPManager::closing;

std::atomic<bool> TCPManager::running(false);
std::mutex TCPManager::mutex;
SafeQueue<shared_ptr<TCPManager::Network_Command>> TCPManager::command_queue;

SafeQueue<shared_ptr<UnifiedState>> TCPManager::unified_state;  // I'm not sure how to get the unified state to the TCPManager

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

int TCPManager::read_command(Network_Command * buffer) {
  uint8_t bytes[2];
  int bytes_read = read(socketfd, bytes, 2);
  buffer->id = (Network_Command_ID) bytes[0];
  buffer->value = bytes[1];
  return bytes_read;
}

int TCPManager::write_data() {
  // TODO write real data
  auto uS = unified_state.dequeue();
  uS->motion_data->x[1] = 32;
  uS->motion_data->x[0] = 27;
  vector<int32_t> vals = { uS->motion_data->x[1], uS->motion_data->x[0] };
  vector<char> bytes = { '9' , ',' , '8' , ',' , '7' };
  return write(socketfd, vals.data(), vals.size() * sizeof(int32_t) + 1);
}

void TCPManager::read_loop() {
  bool active_connection = true;
  Network_Command buffer;
  while (running && active_connection) {
    int bytes_read = read_command(&buffer);
    active_connection = bytes_read > 0;
    if (bytes_read > 0) {
      // print(LogLevel::LOG_EDEBUG, "Bytes read: %d Read command %d %d\n", bytes_read, buffer.id, buffer.value);
      auto command = make_shared<Network_Command>();
      command->id = buffer.id;
      command->value = buffer.value;
      command_queue.enqueue(command);
    }
  }
  print(LogLevel::LOG_INFO, "TCP read Loop exiting.\n");
}

void TCPManager::write_loop() {
  bool active_connection = true;
  while (running && active_connection) {
    closing.wait_for(1000000);
    int written = write_data();
    print(LogLevel::LOG_DEBUG, "Wrote %d bytes\n", written);
    active_connection = written != -1;
  }
  print(LogLevel::LOG_INFO, "TCP write Loop exiting.\n");
}

void TCPManager::tcp_loop(const char * hostname, const char * port) {
  connected.reset();
  closing.reset();
  running.store(true);

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




