#include "UDPManager.hpp"

int UDPManager::socketfd = 0;
std::atomic<bool> UDPManager::running(false);
UDPManager::Connection_Status UDPManager::connection_status = UDPManager::Connection_Status::NOT_YET_CONNECTED;


int UDPManager::start_udp(const char * hostname, const char * port){
  return 0;
}

void UDPManager::connection_monitor(){
  while (running){
    // Do something on the UDP port
  }
}

void UDPManager::close_client() {
  shutdown(socketfd, SHUT_RDWR);
  close(socketfd);
}

void UDPManager::stop_threads() {
  running.store(false);
}
