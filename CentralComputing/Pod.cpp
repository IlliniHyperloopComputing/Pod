#include "Pod.h"

using Utils::print;
using Utils::LogLevel;
using Utils::microseconds;
using std::make_shared;
using std::thread;
using std::function;

Pod::Pod() {
  running.store(false);
  switchVal = false;
}

void Pod::logic_loop() {

  int64_t logic_loop_timeout; // Get the loop sleep (timeout) value
  if (!ConfiguratorManager::config.getValue("logic_loop_timeout", logic_loop_timeout)) {
    print(LogLevel::LOG_ERROR, "Unable to find logic_loop timeout config, exiting logic_loop\n");
    return;
  }

  #ifdef SIM // Used to indicate to the Simulator we have processed a command
  bool command_processed = false;
  #endif

  // Start processing/pod logic
  while (running.load()) {
    auto command = NetworkManager::command_queue.dequeue();

    if (command.get() != nullptr) {
      // Parse the command and call the appropriate state machine function
      auto id = (NetworkManager::Network_Command_ID) command->id;
      auto transition = state_machine->get_transition_function(id);
      ((*state_machine).*(transition))(); 

      #ifdef SIM // Used to indicate to the Simulator that we have processed a command
      command_processed = true;
      #endif
      print(LogLevel::LOG_INFO, "Command : %d %d\n", command->id, command->value);
    } else { // Create a "do nothing" command. This will be passed into the steady state caller below
      command = make_shared<NetworkManager::Network_Command>();
      command->id = 0;
      command->value = 0;
    }

    // Calls the steady state function for the current state
    auto func = state_machine->get_steady_function();
    ((*state_machine).*(func))(command); 

    #ifdef BBB
    // Send the heartbeat signal to the watchdog.
    bool is_GPIO_set = Utils::set_GPIO(HEARTBEAT_GPIO, switchVal);
    if (!is_GPIO_set) {
      print(LOG_ERROR, "GPIO file not being accessed correctly\n");
      // TODO: Add command to command queue
    }
    switchVal = !switchVal;
    #endif

    #ifdef SIM
    // Let the simulator know we processed a command.
    if (command_processed) {
      processing_command.invoke(); 
    }
    command_processed = false;
    #endif 

    // Sleep for the given timeout
    closing.wait_for(logic_loop_timeout);
  } 
   
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
  #ifdef BBB
  // Start up PRU
  if (system("ls /dev | grep rpmsg > /dev/null") != 0) {
    if (system("./initPRU > /dev/null") != 0) {
      print(LogLevel::LOG_ERROR, "PRU not responding\n");
      exit(1);
    }
  }
  print(LogLevel::LOG_INFO, "PRU is on\n");    

  // Set maximum CPU frequency, gotta GO F A S T  
  system("cpufreq-set -f 1000MHz");
  print(LogLevel::LOG_INFO, "CPU freq set to 1GHz\n");    
  #endif

  signal(SIGPIPE, SIG_IGN);
  state_machine = make_shared<Pod_State>();

  print(LogLevel::LOG_INFO, "Pod State: %s\n", state_machine->get_current_state_string().c_str());

  // Start all SourceManager threads
  SourceManager::PRU.initialize();
  SourceManager::CAN.initialize();
  SourceManager::TMP.initialize();
  SourceManager::ADC.initialize();
  SourceManager::I2C.initialize();
  SourceManager::MM.initialize();
  print(LogLevel::LOG_INFO, "Source Managers started\n");

  // Setup Network Server
  string port;
  string local_ip;
  if (!(ConfiguratorManager::config.getValue("tcp_port", port) && 
      ConfiguratorManager::config.getValue("local_ip", local_ip))) {
    print(LogLevel::LOG_ERROR, "Missing tcp_port or local_ip\n");
  }
  NetworkManager::start_server(local_ip.c_str(), port.c_str());

  // Start Network and main loop thread.
  thread network_thread(NetworkManager::network_loop);
  running.store(true);
  thread logic_thread([&](){ logic_loop(); }); // I don't know how to use member functions as a thread function, but lambdas work

  print(LogLevel::LOG_INFO, "Finished Startup\n");
  print(LogLevel::LOG_INFO, "================\n\n");
  
  // ready to begin testing
  ready.invoke();

  // Join all threads
 
  logic_thread.join();
  network_thread.join();
  
  print(LogLevel::LOG_INFO, "Source Managers closing\n");
  // Stop all source managers
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
  // Load the configuration file if specified, or use the default
  string config_to_open;
  if (argc > 1) {
    config_to_open = argv[1];
  } else {
    config_to_open = "defaultConfig.txt";
  }
  if (!ConfiguratorManager::config.openConfigFile(config_to_open)) {
    print(LogLevel::LOG_ERROR, "Config missing. File: %s\n", config_to_open.c_str());
    return 1;
  }

  #ifndef SIM
    Utils::loglevel = LogLevel::LOG_EDEBUG;
    // Create the pod object
    auto pod = make_shared<Pod>();
    // Setup ctrl-c behavior 
    signal(SIGINT, signal_handler);
    shutdown_handler = [&](int signal) { pod->stop(); };
    // Start the pod running
    pod->startup();
    return 0;
  #else
    Utils::loglevel = LogLevel::LOG_EDEBUG;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  #endif
}
