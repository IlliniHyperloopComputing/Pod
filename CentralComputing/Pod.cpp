#include "Pod.h"

#define timestep 10000

using namespace std;
Sensor * sensor;
Spi * spi;
Network * network;
Brake * brake;
Motor * motor;
Pod_State * state_machine;
SafeQueue<Network_Command *> * network_queue;
volatile bool running = true;
long long last_poll; //last time beaglebone polled XMEGA

//Necessary setup for Spi
static uint8_t bpi1_s[] = {2,2,2,2,2};
static uint8_t bpi2_s[] = {4,4,4,4,4,2,2,2,2,2,2};
static uint8_t * bpi1 = bpi1_s;
static uint8_t * bpi2 = bpi2_s;
static Xmega_Setup x1 = {"/dev/spidev1.0", 5, bpi1, 500000, 8};
static Xmega_Setup x2 = {"/dev/spidev1.1", 11, bpi2, 500000, 8};


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
    if(delta > timestep){// TODO Possibly change delta based off state, but at least pick a real timestep
      sensor->update_buffers(); 
    }
    // Start processing/pod logic
    Network_Command * command = network_queue->dequeue();
    if(command != nullptr){
      print_info("Processing command %d\n", command->id);
      auto transition = state_machine->get_transition_function(command->id);
      (state_machine->*(transition))(); //transitions to requested state
    }

    auto func = state_machine->get_steady_function();
    //This is how you call a member function pointer in c++
    (state_machine->*(func))(command); //G E N I U S
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
    spi = new Spi(&x1, &x2);
  #endif


  sensor = new Sensor(spi);
  network = new Network(sensor);
  brake = new Brake();
  motor = new Motor();
  state_machine = new Pod_State(brake, motor, sensor);
  network_queue = new SafeQueue<Network_Command *>();
  const char* host = "127.0.0.1";
  const char* port = "8800";
  network->start_server(host, port);
  Network_Command command = {Network_Command_ID::TRANS_FUNCTIONAL_TEST, 0};
  network_queue->enqueue(&command);

  thread network_thread(network_loop);
  thread logic_thread(logic_loop);
  network_thread.join(); 
  logic_thread.join();
  free(motor);
  free(brake);
  free(network);
  free(sensor);
  free(spi);
  free(state_machine);
  return 0; 
}

