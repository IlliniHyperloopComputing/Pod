#include "Pod.h"

using Utils::print;
using Utils::LogLevel;
using Utils::microseconds;
using std::make_shared;
using std::thread;
using std::function;
using std::shared_ptr;

// Pod's "main loop"
// This logic_loop is the main driver for the pod. 
// It processes commands, interacts with the state machine, 
// collects all of the State data, interacts with the motion model
// organizes what data gets sent to TCP endpoint
void Pod::logic_loop() {
  #ifdef SIM  // Used to indicate to the Simulator we have processed a command
  bool command_processed = false;
  #endif

  // Start processing/pod logic
  while (running.load()) {
    Command::Network_Command com;
    bool loaded = Command::get(&com);

    if (loaded) {
      // Parse the command and call the appropriate state machine function
      auto transition = state_machine->get_transition_function(&com);
      ((*state_machine).*(transition))(); 
      #ifdef SIM  // Used to indicate to the Simulator that we have processed a command
      command_processed = true;
      #endif
      print(LogLevel::LOG_INFO, "Command : %d %d\n", com.id, com.value);
    } else {  // Create a "do nothing" command. This will be passed into the steady state caller below
      com.id = 0;
      com.value = 0;
    }

    // Set error codes if command contained any
    set_error_code(&com);
    // Collect sensor data and set motion model
    update_unified_state();    

    // Calls the steady state function for the current state
    // Passes in command, and current state. 
    auto func = state_machine->get_steady_function();
    ((*state_machine).*(func))(&com, unified_state); 

    #ifdef BBB
    // Send the heartbeat signal to the watchdog.
    bool is_GPIO_set = Utils::set_GPIO(Utils::HEARTBEAT_GPIO, switchVal);
    if (!is_GPIO_set) {
      print(LogLevel::LOG_ERROR, "GPIO file not being accessed correctly\n");
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

    // TODO: enqueue this in a smarter way, so you don't have to pass the entire object
    TCPManager::write_queue.enqueue(unified_state);  

    // Sleep for the given timeout
    closing.wait_for(logic_loop_timeout);
  } 
  print(LogLevel::LOG_INFO, "Exiting Pod Logic Loop\n");
}


void Pod::set_error_code(Command::Network_Command * com) {
  if (com->id >= Command::Network_Command_ID::SET_ADC_ERROR 
    && com->id <= Command::Network_Command_ID::SET_OTHER_ERROR) {
    // Set flag
    unified_state->errors->error_vector[com->id - Command::Network_Command_ID::SET_ADC_ERROR] |= com->value;
  } else if (com->id >= Command::Network_Command_ID::CLR_ADC_ERROR 
    && com->id <= Command::Network_Command_ID::CLR_OTHER_ERROR) {
    // Clear flag
    unified_state->errors->error_vector[com->id - Command::Network_Command_ID::CLR_ADC_ERROR] &= (~com->value);
  }
}

// Helper function called from logic_loop()
// Updates the unified_state
// Calls MotionModel::calculate()
void Pod::update_unified_state() {
  unified_state->adc_data = SourceManager::ADC.Get();
  unified_state->can_data = SourceManager::CAN.Get();
  unified_state->i2c_data = SourceManager::I2C.Get();
  unified_state->pru_data = SourceManager::PRU.Get();
  unified_state->state = state_machine->get_current_state();

  // Update motion_data
  // Pass current state into Motion Model
  #ifndef SIM
  motion_model->calculate(unified_state);
  #else
  motion_model->calculate_sim(unified_state);
  #endif
}

// Pod constructor
// Takes a config file as an argument
// Setsup variables, loads configurations, does some other initialization work. 
// Does NOT start any threads.
Pod::Pod(const std::string & config_to_open) {
  // Setup "0" time. All further calls to microseconds() use this as the base time
  microseconds();

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

  // Open configuration file
  if (!ConfiguratorManager::config.openConfigFile(config_to_open)) {
    print(LogLevel::LOG_ERROR, "Config missing. File: %s\n", config_to_open.c_str());
    exit(1);
  }

  // Grab all configuration variables
  if (!(ConfiguratorManager::config.getValue("tcp_port", tcp_port) && 
      ConfiguratorManager::config.getValue("tcp_addr", tcp_addr) &&
      ConfiguratorManager::config.getValue("udp_send_port", udp_send) &&
      ConfiguratorManager::config.getValue("udp_recv_port", udp_recv) &&
      ConfiguratorManager::config.getValue("udp_addr", udp_addr) &&
      ConfiguratorManager::config.getValue("logic_loop_timeout", logic_loop_timeout))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: Missing necessary configuration\n");
    exit(1);
  }

  // Setup any other member variables here
  state_machine = make_shared<Pod_State>();
  motion_model = make_shared<MotionModel>();
  unified_state = make_shared<UnifiedState>();
  unified_state->motion_data = make_shared<MotionData>();
  unified_state->adc_data = make_shared<ADCData>();
  unified_state->can_data = make_shared<CANData>();
  unified_state->i2c_data = make_shared<I2CData>();
  unified_state->pru_data = make_shared<PRUData>();
  unified_state->errors = make_shared<Errors>();
  running.store(false);
  switchVal = false;
}

// Pod Desctructor
// Clean up Configuration Manager
Pod::~Pod() {
  ConfiguratorManager::config.clear();
}

// Run the Pod.
// Starts all threads going, network threads and SourceManagers
// Joins all threads at the end, and exits gracefully
void Pod::run() {
  print(LogLevel::LOG_INFO, "\n");
  print(LogLevel::LOG_INFO, "==================\n");
  print(LogLevel::LOG_INFO, "ILLINI  HYPERLOOP \n");
  print(LogLevel::LOG_INFO, "==================\n");
  print(LogLevel::LOG_INFO, "Pod State: %s\n", state_machine->get_current_state_string().c_str());

  // Start all SourceManager threads
  SourceManager::PRU.initialize();
  SourceManager::CAN.initialize();
  SourceManager::TMP.initialize();
  SourceManager::ADC.initialize();
  SourceManager::I2C.initialize();

  signal(SIGPIPE, SIG_IGN);  // Ignore SIGPIPE

  // Start Network and main loop thread.
  // I don't know how to use member functions as a thread function, but lambdas work
  thread tcp_thread([&](){ TCPManager::tcp_loop(tcp_addr.c_str(), tcp_port.c_str()); });
  thread udp_thread([&](){ UDPManager::connection_monitor(udp_addr.c_str(), udp_send.c_str(), udp_recv.c_str()); });
  running.store(true);
  thread logic_thread([&](){ logic_loop(); });  
  print(LogLevel::LOG_INFO, "Finished Initialization\n");
  print(LogLevel::LOG_INFO, "================\n\n");
  
  // signal to test suite that we are ready to begin testing
  ready.invoke();

  // Join all threads
  logic_thread.join();
  // Once logic_loop joins, trigger other threads to stop
  TCPManager::close_client();
  UDPManager::close_client();
  tcp_thread.join();
  udp_thread.join();
  SourceManager::PRU.stop();
  SourceManager::CAN.stop();
  SourceManager::TMP.stop();
  SourceManager::ADC.stop();
  SourceManager::I2C.stop();
  print(LogLevel::LOG_INFO, "All threads closed, Pod shutting down\n");
}

// Cause the logic_loop to close.
void Pod::trigger_shutdown() {
  running.store(false);  
  closing.invoke();
}

// Parse any command line arguments passed into the Pod
// Used right now to load the configuration file if specified, or use the default
void parse_command_line_args(int argc, char **argv, string * config_to_open);
void parse_command_line_args(int argc, char **argv, string * config_to_open) {
  *config_to_open = "defaultConfig.txt";
  if (argc > 1) {  // If the first argument is a file, use it as the config file
    ifstream test_if_file(argv[1]);
    if (test_if_file.is_open()) {
      test_if_file.close();
      *config_to_open = argv[1];
    }
  }
}

// Signal handlers. Defined within main()
function<void(int)> shutdown_handler;
void signal_handler(int signal) {shutdown_handler(signal); }

// Main 
// Starts the Pod up, or the GTest suite, depending on compiler flags
int main(int argc, char **argv) {
  std::string config_to_open;

  #ifndef SIM
    Utils::loglevel = LogLevel::LOG_EDEBUG;
    // Load the configuration file if specified, or use the default
    parse_command_line_args(argc, argv, &config_to_open);
    // Create the pod object
    auto pod = make_shared<Pod>(config_to_open);
    // Setup some handlers
    signal(SIGINT, signal_handler);  // ctrl-c handler
    shutdown_handler = [&](int signal) { pod->trigger_shutdown(); };
    // Start the pod running
    pod->run();
    return 0;
  #else
    Utils::loglevel = LogLevel::LOG_EDEBUG;
    testing::InitGoogleTest(&argc, argv);  // after calling this, all argc/v related to gtest are removed
    parse_command_line_args(argc, argv, &config_to_open);
    podtest_global::config_to_open = config_to_open;
    return RUN_ALL_TESTS();
  #endif
}

// This is used in tests/PodTest.cpp
// Needed to pass in variables into the testing environment
// Used to pass config file name, specifically
// See Pod.h for where this namespace is declared.
namespace podtest_global {
  std::string config_to_open;
}  // namespace podtest_global
