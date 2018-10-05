#include "Pod.h"

using namespace std;
using namespace Utils;


Pod::Pod() {
  running.store(false);
}

void Pod::logic_loop() {
	while(running.load()){
    // Start processing/pod logic
		shared_ptr<NetworkManager::Network_Command> command = NetworkManager::command_queue.dequeue();
    bool command_processed = false;
    if(command.get() != nullptr){
      //print(LogLevel::LOG_INFO, "Command : %d %d\n", command->id, command->value);
      NetworkManager::Network_Command_ID id = (NetworkManager::Network_Command_ID) command->id;
      auto transition = state_machine->get_transition_function(id);
      ((*state_machine).*(transition))(); //transitions to requested state
      command_processed = true;
    } else {
     //print(LogLevel::LOG_INFO, "No Command\n");
      command = make_shared<NetworkManager::Network_Command>();
      command->id = 0;
      command->value = 0;
    }

    auto func = state_machine->get_steady_function();
    ((*state_machine).*(func))(command); //calls the steady state function for the current state

    if(command_processed){
      processing_command.invoke(); 
    }
    closing.wait_for(1000);
  } 
  //process all commands before closing
   
  print(LogLevel::LOG_INFO, "Exiting Pod Logic Loop\n");
}

void Pod::startup() {
  microseconds();
 	print(LogLevel::LOG_INFO, "\n");
 	print(LogLevel::LOG_INFO, "==================\n");
  print(LogLevel::LOG_INFO, "ILLINI  HYPERLOOP \n");
  print(LogLevel::LOG_INFO, "==================\n");
  print(LogLevel::LOG_INFO, "Running Startup\n");

  // If we are on the BBB, run specific setup
  if(system("hostname | grep beaglebone > /dev/null") == 0){

    // Start up PRU
    if(system("ls /dev | grep rpmsg > /dev/null") != 0){
      if(system("./initPRU > /dev/null") != 0){
        print(LogLevel::LOG_ERROR, "PRU not responding\n");
        exit(1);
      }
    }
    print(LogLevel::LOG_INFO, "PRU is on\n");    

    // Set maximum CPU frequency, gotta GO F A S T  
    system("cpufreq-set -f 1000MHz");
    print(LogLevel::LOG_INFO, "CPU freq set to 1GHz\n");    
  }

  signal(SIGPIPE, SIG_IGN);
  state_machine = make_shared<Pod_State>();

  print(LogLevel::LOG_INFO, "Pod State: %s\n", state_machine->get_current_state_string().c_str());

  //Start all SourceManager threads
  SourceManager::PRU.initialize();
  SourceManager::CAN.initialize();
  SourceManager::TMP.initialize();
  SourceManager::ADC.initialize();
  SourceManager::I2C.initialize();
  SourceManager::MM.initialize();
  print(LogLevel::LOG_INFO, "Source Managers started\n");

  //Setup Network Server
  NetworkManager::start_server("127.0.0.1", "8800");

  //Start Network and main loop thread.
  thread network_thread(NetworkManager::network_loop);
  running.store(true);
  thread logic_thread([&](){ logic_loop(); }); // I don't know how to use member functions as a thread function, but lambdas work

  print(LogLevel::LOG_INFO, "Finished Startup\n");
  print(LogLevel::LOG_INFO, "================\n\n");
  
  //ready to begin testing
  ready.invoke();

  //Join all threads
 
  logic_thread.join();
  network_thread.join();
  
  print(LogLevel::LOG_INFO, "Source Managers closing\n");
  //Stop all source managers
  SourceManager::MM.stop(); // Must be called first
  SourceManager::PRU.stop();
  SourceManager::CAN.stop();
  SourceManager::TMP.stop();
  SourceManager::ADC.stop();
  SourceManager::I2C.stop();
  print(LogLevel::LOG_INFO, "Source Managers closed, Pod shutting down\n");

}

void Pod::stop() {
  running.store(false); 
  closing.invoke();
  NetworkManager::stop_threads();
  NetworkManager::close_server();
}

function<void(int)> shutdown_handler;
void signal_handler(int signal) {shutdown_handler(signal); }

int main(int argc, char **argv) {
  #ifndef SIM

    Utils::loglevel = LOG_EDEBUG;
    auto pod = make_shared<Pod>();
    signal(SIGINT, signal_handler);
    shutdown_handler = [&](int signal) {
      pod->stop();
    };
    pod->startup();
  #else
    Utils::loglevel = LOG_EDEBUG;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  #endif
}
