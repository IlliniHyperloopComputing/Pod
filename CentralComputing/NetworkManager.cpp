#include "NetworkManager.hpp"

using namespace std;

int NetworkManager::socketfd = 0;
int NetworkManager::clientfd = 0;
int NetworkManager::udp_socket = 0;

std::atomic<bool> NetworkManager::running(false);
sockaddr_storage NetworkManager::addr_dest = {};
SafeQueue<shared_ptr<NetworkManager::Network_Command>> NetworkManager::command_queue;

uint8_t NetworkManager::start_server(const char * hostname, const char * port) {
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  int enable = 1;
  int blocking = 0;
  ioctl(socketfd, FIONBIO, &blocking);
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    perror("setsockopt(SO_REUSEADDR) failed");
  }

  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int s = getaddrinfo(NULL, port, &hints, &result);
  if(s != 0){
    print_info("getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }
  if(bind(socketfd, result->ai_addr, result->ai_addrlen) != 0){
    perror("bind");
    exit(1);
  }
  if(listen(socketfd, 1) != 0){
    perror("listen");
    exit(1);
  }
  print_info("Server setup successfully\n");
  free(result);

  running.store(true);
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
  print_info("Awaiting connection\n");
  while(1) {
    ret = poll(&p, 1, 1000);
    if(ret == 1) {//there's something trying to connect
      clientfd = accept(socketfd, NULL, NULL);
      print_info("Connected!\n"); 
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
  // TODO 
  // Write ParameterManager::GetNetworkReport, write the report to clientfd
  return -1;
}

void NetworkManager::close_server() {
  close(clientfd);
  close(socketfd);
}

void NetworkManager::send_packet() {

}

void NetworkManager::network_loop() {
  while(running){
    int fd = accept_client();
    print_info("Client fd is: %d\n", clientfd);
    if(fd > 0){
      print_info("Starting network threads\n");
      thread read_thread(read_loop);
      thread write_thread(write_loop);

      read_thread.join();
      write_thread.join();
      print_info("Client exited, looking for next client\n");

    } else {
      PRINT_ERRNO("Accept failed, abort.")
      break;
    }
  }   
}

void NetworkManager::read_loop() {
  bool active_connection = true;
  Network_Command buffer;
  while (running && active_connection){
    int bytes_read = read_command(&buffer);
    active_connection = bytes_read > 0;
    if (bytes_read > 0) {
      auto command = make_shared<Network_Command>();
      command->id = buffer.id;
      command->value = buffer.value;
      command_queue.enqueue(command);
    }
  }

  print_info("Read Loop exiting.\n");
}

void NetworkManager::write_loop() {
  bool active_connection = true;
  while(running && active_connection){
    usleep(100000); //TODO: Change to actual value at some point
    int written = write_data();
    active_connection = written != -1;
  }

  print_info("Write Loop exiting.\n");
}

void NetworkManager::stop_threads() {
  running.store(false);
}



