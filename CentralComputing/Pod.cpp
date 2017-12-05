#include "Pod.h"
#include "Spi.h"

#define timestep 10000

using namespace std;
Spi * spi;
Sensor * sensor;
Network * network;
Xmega * xmega;
volatile bool running = true;
long long last_poll; //last time beaglebone polled XMEGA


void write_loop(){
  bool active_connection = true;
  while(running && active_connection){
    usleep(100000); //TODO: Change to actual value at some point
    int written = network->write_data();
    print_debug("Written is %d\n", written);
    active_connection = written != -1;
  }

  PRINT_ERRNO("Write Loop exiting.")
}

void read_loop(){
  bool active_connection = true;
  Network_Command buffer;
  while (running && active_connection){
    int bytes_read = network->read_command(&buffer);
    active_connection = bytes_read != -1;
  }

  PRINT_ERRNO("Read Loop exiting.")
}
void network_loop(){
  while(running){ 
    int clientfd = network->accept_client(); 
    print_info("Client fd is: %d\n", clientfd);
    if(clientfd > 0){
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

void logic_loop(){
  while(running){
    long long now = get_elapsed_time(); 
    long long delta = now - last_poll;
    if(delta > timestep){
      Xmega_Command command = xmega->transfer();
      if(command != X_NONE){
        print_info("Command %s sent at time %d\n", xmega->x_command_to_string(command), now);
      }
      sensor->update_buffers();
      // TODO PID loop

    }
    //TODO: Change to actual value at some point
    
    usleep(100);//Need to yield, otherwise this will block other threads
  }
}

void int_handler(int signo){
  network->close_server();
  running = false;
}

void pipe_handler(int signo){

}

int main(){

  // parsing, setup, etc
  signal(SIGINT, int_handler);
  signal(SIGPIPE, pipe_handler);
  spi = NULL;
  #ifndef SIM
  // setup SPI 
  #endif
 

  xmega = new Xmega(spi); 
  sensor = new Sensor(spi);
  network = new Network(sensor);
  const char* host = "127.0.0.1";
  const char* port = "8800";
  network->start_server(host, port);

  thread network_thread(network_loop);
  thread logic_thread(logic_loop);
  network_thread.join(); 
  logic_thread.join();
  return 0; 
}

