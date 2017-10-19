#ifndef SIM
#include "Network.h"

Network::Network(Sensor * sen) : sensor(sen) {

}

uint8_t Network::start_server(const char * hostname, const char * port){
  int socketfd = socket(AF_INET, SOCK_STREAM, 0);
  int enable = 1;
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    perror("setsockopt(SO_REUSEADDR) failed");
  }

  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int s = getaddrinfo(NULL, "8800", &hints, &result);
  if(s != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
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
  free(result);
}

uint8_t Network::start_udp(const char * hostname, const char * port){
  return 0;
}

int Network::accept(){
  return 0;
}



int Network::read_command(Network_Command *){
  return 0;
}

int Network::write_data() {
  return 0;
}

void Network::close_server() {

}

void Network::send_packet() {

}
#endif
