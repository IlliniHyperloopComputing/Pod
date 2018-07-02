#include "Pod.h"



using namespace std;

shared_ptr<Pod_State> state_machine;
SafeQueue<shared_ptr<NetworkManager::Network_Command>> command_queue;
bool running = true;

void logic_loop() {
	while(running){
    // Start processing/pod logic
		shared_ptr<NetworkManager::Network_Command> command = command_queue.dequeue();
    if(command != nullptr){
      print_info("Processing command %d\n", command->id);
      auto transition = state_machine->get_transition_function(command->id);
      ((*state_machine).*(transition))(); //transitions to requested state
    }

    auto func = state_machine->get_steady_function();
    //This is how you call a member function pointer in c++
    ((*state_machine).*(func))(command); //G E N I U S
  } 
}

int main() {
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
  network_thread.join();

}
