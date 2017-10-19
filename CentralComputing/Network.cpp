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
  print_debug("Server setup successfully\n");
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
  print_debug("Awaiting connection\n");
  while(1) {
    ret = poll(&p, 1, 1000);
    if(ret == 1) {//there's something trying to connect
      clientfd = accept(socketfd, NULL, NULL);
      print_debug("Connected!\n"); 
      return clientfd;
    }
  }
  return -1;
}



int Network::read_command(Network_Command *){
  return 0;
}

int Network::write_data() {
  Arbitrary_Data buffer = null_data;
  size_t used = 0;
  for(size_t i = 0; i < Data_ID::NULL_ID; i++){
    Data_ID id = (Data_ID) i;
    Data d = sensor->get_data(id);  
    used += append(buffer, used, d.calculated);
    used += append(buffer, used, d.raw);
    cleanup(d);
  }
  int bytes = write_all_to_socket(socketfd, buffer.data, used);   
  cleanup(buffer);
  return bytes;
}
void Network::close_server() {

}

void Network::send_packet() {

}
