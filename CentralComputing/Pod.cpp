#include "Pod.h"

using namespace std;

PRUManager SourceManager::PRU(1.0);

shared_ptr<Pod_State> state_machine;
bool running = true;

void logic_loop() {
	while(running){
    // Start processing/pod logic
		shared_ptr<NetworkManager::Network_Command> command = NetworkManager::command_queue.dequeue();
    if(command.get() != nullptr){
      print_info("Command : %d %d\n", command->id, command->value);
      print_info("Processing command %d\n", command->id);
      auto transition = state_machine->get_transition_function(command->id);
      ((*state_machine).*(transition))(); //transitions to requested state
    } else {
      print_info("No Command\n");
      command = make_shared<NetworkManager::Network_Command>();
      command->id = (NetworkManager::Network_Command_ID) 0;
      command->value = 0;
    }

    auto func = state_machine->get_steady_function();
    ((*state_machine).*(func))(command); //calls the steady state function for the current state
    sleep(5);
  } 
}

int main() {
  microseconds();
 	print_info("==================\n");
  print_info("ILLINI  HYPERLOOP \n");
  print_info("==================\n\n");
  print_info("Runing setup...\n");

  //Set maximum CPU frequency because GOTTA GO F A S T  
  system("cpufreq-set -f 1000MHz");
  print_info("CPU freq set to 1GHz\n");    
  state_machine = make_shared<Pod_State>();
  
 

  NetworkManager::start_server("127.0.0.1", "8800");
  thread network_thread(NetworkManager::network_loop);
  thread logic_thread(logic_loop);
  logic_thread.join();
  network_thread.join();

}
