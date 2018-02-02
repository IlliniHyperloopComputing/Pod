#include "Pod.h"
#include "SafeQueue.h"

#define timestep 10000

using namespace std;
Spi * spi;
Sensor * sensor;
Network * network;
Xmega * xmega;
Brake * brake;
Motor * motor;
Pod_State * state_machine;
SafeQueue<Network_Command *> * network_queue;
volatile bool running = true;
long long last_poll; //last time beaglebone polled XMEGA

std::map<Pod_State::E_States, steady_state_function> function_map;

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
    Network_Command * command = new Network_Command(buffer);
    network_queue->enqueue(command);
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
    if(delta > timestep){// TODO Possibly change delta based off state, but at 
      Xmega_Command command = xmega->transfer();
      if(command != X_NONE){
        print_info("Command %s sent at time %d\n", xmega->x_command_to_string(command), now);
      }
      sensor->update_buffers(); 
    }
    
    usleep(delta);//Need to yield, otherwise this will block other threads
    Network_Command * command = network_queue->dequeue();
    //Network_Command command = { Network_Command_ID::TRANS_SAFE_MODE, 0xAB };
    auto state = state_machine->get_current_state();
    auto func = function_map[state];
    (state_machine->*(func))(command);
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
  spi = nullptr;
  #ifndef SIM
  // setup SPI 
  #endif
 
  
  xmega = new Xmega(spi); 
  sensor = new Sensor(spi);
  network = new Network(sensor);
  brake = new Brake();
  motor = new Motor();
  state_machine = new Pod_State();
  network_queue = new SafeQueue<Network_Command *>();
  const char* host = "127.0.0.1";
  const char* port = "8800";
  network->start_server(host, port);


  thread network_thread(network_loop);
  thread logic_thread(logic_loop);
  network_thread.join(); 
  logic_thread.join();
  free(motor);
  free(brake);
  free(network);
  free(sensor);
  free(xmega);
  free(spi);
  free(state_machine);
  return 0; 
}

