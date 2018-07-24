#include "Simulator.hpp"

Simulator::Simulator(const char * logfile) : logpath(logfile) {
  //TODO set up internal variables
}

bool Simulator::connect(const char * hostname, const char * port) {
  //TODO connect to a Pod instance
  
  active_connection.store(true);
  return false;
}

bool Simulator::send_command(shared_ptr<NetworkManager::Network_Command> command) {
  int bytes_written = write(socketfd, command.get(), sizeof(NetworkManager::Network_Command));
  return bytes_written == sizeof(NetworkManager::Network_Command);

}

void Simulator::read_loop() {
  while(active_connection.load()){
    //TODO figure out how we want to actually send data over the network
  }
}

void Simulator::disconnect() {
  active_connection.store(false);
}
