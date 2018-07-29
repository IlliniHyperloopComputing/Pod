#include "Pod.h"

using namespace std;
using namespace Utils;
Pod::Pod() {
  running.store(false);
  sem_init(&ready_semaphore, 0, 0);
}

void Pod::logic_loop() {
	while(running.load()){
    // Start processing/pod logic
		shared_ptr<NetworkManager::Network_Command> command = NetworkManager::command_queue.dequeue();
    if(command.get() != nullptr){
      print(LogLevel::LOG_INFO, "Command : %d %d\n", command->id, command->value);
      print(LogLevel::LOG_INFO, "Processing command %d\n", command->id);
      auto transition = state_machine->get_transition_function(command->id);
      ((*state_machine).*(transition))(); //transitions to requested state
    } else {
      print(LogLevel::LOG_INFO, "No Command\n");
      command = make_shared<NetworkManager::Network_Command>();
      command->id = (NetworkManager::Network_Command_ID) 0;
      command->value = 0;
    }

    auto func = state_machine->get_steady_function();
    ((*state_machine).*(func))(command); //calls the steady state function for the current state
    sleep(1);
  } 
  print(LogLevel::LOG_INFO, "Exiting Pod Logic Loop\n");
}

void Pod::startup() {
  microseconds();
 	print(LogLevel::LOG_INFO, "==================\n");
  print(LogLevel::LOG_INFO, "ILLINI  HYPERLOOP \n");
  print(LogLevel::LOG_INFO, "==================\n\n");
  print(LogLevel::LOG_INFO, "Running setup...\n");

  //Set maximum CPU frequency because GOTTA GO F A S T  
  system("cpufreq-set -f 1000MHz");
  print(LogLevel::LOG_INFO, "CPU freq set to 1GHz\n");    
  state_machine = make_shared<Pod_State>();

  print(LogLevel::LOG_INFO, "Pod State: %s\n", state_machine->get_current_state_string().c_str());

  //Start all SourceManager threads
  SourceManager::PRU.initialize();
  SourceManager::CAN.initialize();
  SourceManager::TMP.initialize();
  SourceManager::ADC.initialize();
  SourceManager::I2C.initialize();

  //Setup Network Server
  NetworkManager::start_server("127.0.0.1", "8800");

  //Start Network and main loop thread.
  thread network_thread(NetworkManager::network_loop);
  running.store(true);
  thread logic_thread([&](){ logic_loop(); }); // I don't know how to use member functions as a thread function, but lambdas work
  
  //ready to begin testing
  sem_post(&ready_semaphore);

  //Join all threads
  logic_thread.join();
  network_thread.join();
  
  print(LogLevel::LOG_INFO, "Source Managers closing\n");
  //Stop all source managers
  SourceManager::PRU.stop();
  SourceManager::CAN.stop();
  SourceManager::TMP.stop();
  SourceManager::ADC.stop();
  SourceManager::I2C.stop();
  print(LogLevel::LOG_INFO, "Source Managers closed, Pod shutting down\n");

}

void Pod::stop() {
  running.store(false); 
  NetworkManager::stop_threads();
  NetworkManager::close_server();
}

int main(int argc, char **argv) {
  #ifndef SIM
    Utils::loglevel = LOG_EDEBUG;
    auto pod = make_shared<Pod>();
    pod->startup();
  #else
    Utils::loglevel = LOG_EDEBUG;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  #endif
}
