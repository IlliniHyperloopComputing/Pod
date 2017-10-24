#ifdef SIM
#include "Network.h"

Network::Network(Sensor * sen) {

}

uint8_t Network::start_server(const char * hostname, const char * port){
  return 0;
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
