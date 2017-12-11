#include "Network.h"
#include "Utils.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>

Network::Network(Sensor * sen) : sensor(sen) {

}

uint8_t Network::start_server(const char * hostname, const char * port){
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  int enable = 1;
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
  return socketfd;
}

uint8_t Network::start_udp(const char * hostname, const char * port){
  return 0;
}

int Network::accept_client(){
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



int Network::read_command(Network_Command * buffer){
  return read(clientfd, buffer, sizeof(Network_Command));
}

int Network::write_data() {

  size_t capacity = 500;
  size_t size = 0; 
  uint8_t * buffer = (uint8_t *) malloc(capacity);

  for(size_t i = 0; i < Data_ID::STATE_ID; i++){
    Data_ID id = (Data_ID) i;
    Data d = sensor->get_data(id);  
    size_t delta = 1 + d.raw.size + d.calculated.size; //1 byte for the data_id
    if(size + delta > capacity) {
      buffer = (uint8_t *) realloc(buffer, 2 * (size + delta));
      capacity = 2 * (size + delta);
    }
    uint8_t byte = id;
    memcpy(buffer + size, &byte, 1);
    memcpy(buffer + size + 1, d.calculated.data, d.calculated.size);
    memcpy(buffer + size + 1 + d.calculated.size, d.raw.data, d.raw.size);
    size += delta;
    cleanup(d);
  }
  int bytes = write(clientfd, buffer, size);   
  free(buffer);
  return bytes;
}
void Network::close_server() {
  close(clientfd);
  close(socketfd);
}

void Network::send_packet() {

}
