#include "NetworkManager.hpp"

using namespace std;
using namespace Utils;

int NetworkManager::socketfd = 0;
int NetworkManager::clientfd = 0;
int NetworkManager::udp_socket = 0;
std::string NetworkManager::hostname;
std::string NetworkManager::port;

Event NetworkManager::connected;
Event NetworkManager::closing;

std::atomic<bool> NetworkManager::running(false);
SafeQueue<shared_ptr<NetworkManager::Network_Command>> NetworkManager::command_queue;

uint8_t NetworkManager::start_tcp(const char * _hostname, const char * _port) {
  connected.reset();
  closing.reset();
  std::string h(_hostname);
  hostname = h;
  std::string p(_port);
  port = p;

  if(connect_to_server() > 0){
    print(LogLevel::LOG_INFO, "Client connection successful\n");
  }

  running.store(true);
  return socketfd;
}


int NetworkManager::connect_to_server(){

  struct addrinfo hints, *servinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int rv;
  if((rv = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
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

  return socketfd;
}

uint8_t NetworkManager::start_udp(const char * hostname, const char * port) {
  return 0;
}

int NetworkManager::accept_client() {
  struct pollfd p;
  p.fd = socketfd;
  p.events = POLLIN;
  int ret = 0;
  print(LogLevel::LOG_INFO, "Awaiting connection\n");
  while(1) {
    ret = poll(&p, 1, 200);
    if(ret == 1) {//there's something trying to connect, or we are exiting
      clientfd = accept(socketfd, NULL, NULL);
      if(clientfd != -1)
        print(LogLevel::LOG_INFO, "Connected!\n"); 
      return clientfd;
    }
  }
  return -1;
}

int NetworkManager::read_command(Network_Command * buffer) {
  //int bytes_read = read(clientfd, buffer, sizeof(Network_Command));
  uint8_t bytes[2];
  int bytes_read = read(clientfd, bytes, 2);
  buffer->id = (Network_Command_ID) bytes[0];
  buffer->value = bytes[1];
 
  return bytes_read;
}

int NetworkManager::write_data() {
  // TODO write real data
  vector<uint8_t> bytes = { static_cast<uint8_t>(0xdeadbeef) };
  return write(clientfd, bytes.data(), bytes.size());
}

void NetworkManager::close_server() {
  close(clientfd);
  close(socketfd);
}

void NetworkManager::send_packet() {

}

void NetworkManager::network_loop() {
  while(running){
    int fd = connect_to_server();
    if(fd > 0){
      print(LogLevel::LOG_INFO, "Starting network threads\n");
      thread read_thread(read_loop);
      thread write_thread(write_loop);

      connected.invoke();
      read_thread.join();
      write_thread.join();
      print(LogLevel::LOG_INFO, "Client exited, looking for next client\n");

    } else {
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
      //print(LogLevel::LOG_INFO, "Bytes read: %d Read command %d %d\n", bytes_read, buffer.id, buffer.value);
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
    closing.wait_for(100000);
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



