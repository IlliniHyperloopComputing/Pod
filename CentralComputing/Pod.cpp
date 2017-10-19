#include "Pod.h"

using namespace std;
Sensor * sensor;
Network * network;
Xmega * xmega;
bool running = true;


void write_loop(){
  int written = 0;
  while(running && written >= 0){
    written = network->write_data();
  }
}

void read_loop(){
  int read = 0; 
  Network_Command buffer;
  while (running && read >= 0){
    read = network->read_command(&buffer);
    //Parse buffer and perform actions
  }
}
void network_loop(){
  while(running){ 
    int clientfd = network->accept_client(); 
    if(clientfd > 0){
      thread read_thread(read_loop);
      thread write_thread(write_loop);

      read_thread.join();
      write_thread.join();
    } else {
      cout << "Accept failed, aborting" << endl; 
      break;
    }

  }
}

void xmega_loop(){
  while(running){
    xmega->read();
    sensor->update_buffers();
  }
}

int main(){

  // parsing, setup, etc
  xmega = new Xmega(); 
  sensor = new Sensor(xmega);
  network = new Network(sensor);
  const char* host = "127.0.0.1";
  const char* port = "8080";
  network->start_server(host, port);

  thread network_thread(network_loop);
  thread xmega_thread(xmega_loop);
  network_thread.join(); 
  return 0;
  
}
