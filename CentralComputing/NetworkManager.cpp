#include "NetworkManager.hpp"

using namespace std;
using namespace Utils;

int NetworkManager::socketfd = 0;
int NetworkManager::udp_socket = 0;
std::string NetworkManager::hostname;
std::string NetworkManager::port;

Event NetworkManager::connected;
Event NetworkManager::closing;

std::atomic<bool> NetworkManager::running(false);
SafeQueue<shared_ptr<NetworkManager::Network_Command>> NetworkManager::command_queue;

int NetworkManager::connect_to_server( const char * hostname, const char * port){
  struct addrinfo hints, *servinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int rv;
  if((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error get addrinfo\n");
    return false;
  }

  if((socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error getting socket\n");
    return false;
  }

  if(connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    close(socketfd);
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error connecting\n");
    return false;
  }

  freeaddrinfo(servinfo);
  return socketfd;
}

uint8_t NetworkManager::start_udp(const char * hostname, const char * port) {
  return 0;
}

int NetworkManager::read_command(Network_Command * buffer) {
  uint8_t bytes[2];
  int bytes_read = read(socketfd, bytes, 2);
  buffer->id = (Network_Command_ID) bytes[0];
  buffer->value = bytes[1];
 
  return bytes_read;
}

int NetworkManager::write_data() {
  // TODO write real data
  vector<uint8_t> bytes = { static_cast<uint8_t>(0xdeadbeef) };
  return write(socketfd, bytes.data(), bytes.size());
}

void NetworkManager::close_client() {
  close(socketfd);
  connected.reset();
}

void NetworkManager::tcp_loop(const char * hostname, const char * port) {
  connected.reset();
  closing.reset();
  running.store(true);

  while(running){
    int fd = connect_to_server(hostname, port);
    if(fd > 0){
      print(LogLevel::LOG_INFO, "Starting network threads\n");
      thread read_thread(read_loop);
      thread write_thread(write_loop);

      connected.invoke(); // Threads started, show simulator we are connected

      read_thread.join();
      write_thread.join();
      print(LogLevel::LOG_INFO, "Connection lost\n");

    } 
    else {
      running.store(false);
      break;
    }
  }   

  print(LogLevel::LOG_INFO, "Exiting Network loop\n");
}

void NetworkManager::read_loop() {
  bool active_connection = true;
  Network_Command buffer;
  while (running && active_connection){
    int bytes_read = read_command(&buffer);
    active_connection = bytes_read > 0;
    if (bytes_read > 0) {
      print(LogLevel::LOG_INFO, "Bytes read: %d Read command %d %d\n", bytes_read, buffer.id, buffer.value);
      auto command = make_shared<Network_Command>();
      command->id = buffer.id;
      command->value = buffer.value;
      command_queue.enqueue(command);
    }
  }

  print(LogLevel::LOG_INFO, "Read Loop exiting.\n");
}

void NetworkManager::write_loop() {
  bool active_connection = true;
  while(running && active_connection){
    closing.wait_for(1000000);
    int written = write_data();
    //print(LogLevel::LOG_EDEBUG, "Wrote %d bytes\n", written);
    active_connection = written != -1;
  }

  print(LogLevel::LOG_INFO, "Write Loop exiting.\n");
}

void NetworkManager::stop_threads() {
  running.store(false);
  closing.invoke();
}



