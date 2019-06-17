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
  bool error_processed = false;
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
      if (!(com.id >= Command::Network_Command_ID::SET_ADC_ERROR &&
            com.id <= Command::Network_Command_ID::CLR_OTHER_ERROR)) {
        command_processed = true;  // Processed a command, not an error
      } else {
        error_processed = true;  // Processed an error, not a command
      }
      #endif
      print(LogLevel::LOG_INFO, "Command : %d %d\n", com.id, com.value);
    } else {  // Create a "do nothing" command. This will be passed into the steady state caller below
      com.id = 0;
      com.value = 0;
    }

    TCPManager::data_mutex.lock();
    // Set error codes if command contained any
    set_error_code(&com);
    // Collect sensor data and set motion model
    update_unified_state();    
    TCPManager::data_mutex.unlock();

    // Calls the steady state function for the current state
    // Passes in command, and current state. 
    auto func = state_machine->get_steady_function();
    ((*state_machine).*(func))(&com, &unified_state); 

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

    if (error_processed) {
      processing_error.invoke(); 
    }
    error_processed = false;
    #endif 

    // Sleep for the given timeout
    closing.wait_for(logic_loop_timeout);
  } 
  print(LogLevel::LOG_INFO, "Exiting Pod Logic Loop\n");
}


// Helper function called from logic_loop()
// Updates the unified_state based on if there is SET or CLR error code
void Pod::set_error_code(Command::Network_Command * com) {
  if (com->id >= Command::Network_Command_ID::SET_ADC_ERROR 
    && com->id <= Command::Network_Command_ID::SET_OTHER_ERROR) {
    // Set flag
    unified_state.errors->error_vector[com->id - Command::Network_Command_ID::SET_ADC_ERROR] |= com->value;
  } else if (com->id >= Command::Network_Command_ID::CLR_ADC_ERROR 
    && com->id <= Command::Network_Command_ID::CLR_OTHER_ERROR) {
    // Clear flag
    unified_state.errors->error_vector[com->id - Command::Network_Command_ID::CLR_ADC_ERROR] &= (~com->value);
  }
}

// Helper function called from logic_loop()
// Updates the unified_state
// Calls MotionModel::calculate()
void Pod::update_unified_state() {
  unified_state.adc_data = SourceManager::ADC.Get();
  unified_state.can_data = SourceManager::CAN.Get();
  unified_state.i2c_data = SourceManager::I2C.Get();
  unified_state.pru_data = SourceManager::PRU.Get();
  unified_state.state = state_machine->get_current_state();
  // unified_state->errors is already updated, see 'set_error_code`

  // Update motion_data
  // Pass current state into Motion Model
  #ifndef SIM
  motion_model->calculate(&unified_state);
  #else
  motion_model->calculate_sim(&unified_state);
  #endif
}

// Pod constructor
// Takes config files as arguments
// Setsup variables, loads configurations, does some other initialization work. 
// Does NOT start any threads.
Pod::Pod(const std::string & config_to_open, const std::string & flight_plan_to_open) {
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

  // Open flight plan file
  if (!ConfiguratorManager::config.openConfigFile(flight_plan_to_open, true)) {
    print(LogLevel::LOG_ERROR, "Flight Plan missing. File: %s\n", flight_plan_to_open.c_str());
    exit(1);
  }

  // Open configuration file
  if (!ConfiguratorManager::config.openConfigFile(config_to_open, false)) {
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
    exit(1);  // Crash hard on this error
  }

  // Setup any other member variables here
  state_machine = make_shared<Pod_State>();
  motion_model = make_shared<MotionModel>();
  unified_state.motion_data = make_shared<MotionData>();
  unified_state.adc_data = make_shared<ADCData>();
  unified_state.can_data = make_shared<CANData>();
  unified_state.i2c_data = make_shared<I2CData>();
  unified_state.pru_data = make_shared<PRUData>();
  unified_state.errors = make_shared<Errors>();
  unified_state.state = E_States::ST_SAFE_MODE;
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

  // Transition into SafeMode
  // technically we are already in SafeMode. However, when initialized the 
  // ST_Safe_Mode() transition function isn't called. 
  // This function turns off the motor, and safes the HV power system
  // There could be instances where doing this is beneficial, to remove any sort
  // of transients. 
  Command::Network_Command com;
  com.id = Command::Network_Command_ID::TRANS_SAFE_MODE;
  com.value = 0;
  auto transition = state_machine->get_transition_function(&com);
  ((*state_machine).*(transition))(); 

  // Ignore SIGPIPE. Generated from networking events, and can cause crashes if not ignored.
  signal(SIGPIPE, SIG_IGN);  

  // Initially have an error set that the network isn't connected
  // Connecting to the network will clear this error
  com.id = Command::Network_Command_ID::SET_NETWORK_ERROR;
  com.value = NETWORKErrors::TCP_DISCONNECT_ERROR;
  set_error_code(&com);  

  // Start Network and main loop thread.
  print(LogLevel::LOG_INFO, "tcp_addr: %s, tcp_port: %s \n", 
                            tcp_addr.c_str(), tcp_port.c_str()); 
  print(LogLevel::LOG_INFO, "upd_addr: %s, upd_send: %s, udp_recv: %s\n", 
                            udp_addr.c_str(), udp_send.c_str(), udp_recv.c_str());    

  // I don't know how to use member functions as a thread function, but lambdas work
  // std::lock_guard<std::mutex> guard(TCPManager::data_mutex);  // Protect access to TCPManger::data_to_send
  thread tcp_thread([&](){ TCPManager::tcp_loop(tcp_addr.c_str(), tcp_port.c_str(), &unified_state); });
  thread udp_thread([&](){ UDPManager::connection_monitor(udp_addr.c_str(), udp_send.c_str(), udp_recv.c_str()); });
  running.store(true);
  thread logic_thread([&](){ logic_loop(); });  
  print(LogLevel::LOG_INFO, "Finished Initialization\n");
  print(LogLevel::LOG_INFO, "================\n");
  
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
void parse_command_line_args(int argc, char **argv, string * config_to_open, string * flight_plan_to_open);
void parse_command_line_args(int argc, char **argv, string * config_to_open, string * flight_plan_to_open) {
  *config_to_open      = "defaultConfig.txt";
  *flight_plan_to_open = "defaultFlightPlan.txt";
  for (int i = 1; i < argc; i+=2) {
    // basic help
    if (strncmp(argv[i], "-h", 2) == 0 || strncmp(argv[i], "--help", 6) == 0 ||  strncmp(argv[i], "--h", 3) == 0) {
      print(LogLevel::LOG_INFO, "-c [config_file.txt]\n -f [flight_plan_file.txt]\n");
      exit(1);
    }

    ifstream test_if_file(argv[i+1]);
    if (test_if_file.is_open()) {
      test_if_file.close();
      if (strncmp(argv[i], "-c", 2) == 0) {
        // Configuration file
        *config_to_open = argv[i+1];
      } else if (strncmp(argv[i], "-f", 2) == 0) {
        // Flight plan
        *flight_plan_to_open = argv[i+1];
      } else {
        print(LogLevel::LOG_ERROR, "Invalid command line option\n");
        exit(1);
      }
    } else {
      print(LogLevel::LOG_ERROR, "Not a real file / Invalid command line option\n");
      exit(1);
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
  std::string flight_plan_to_open;

  #ifndef SIM
    Utils::loglevel = LogLevel::LOG_EDEBUG;
    // Load the configuration file if specified, or use the default
    parse_command_line_args(argc, argv, &config_to_open, &flight_plan_to_open);
    // Create the pod object
    auto pod = make_shared<Pod>(config_to_open, flight_plan_to_open);
    // Setup some handlers
    signal(SIGINT, signal_handler);  // ctrl-c handler
    shutdown_handler = [&](int signal) { pod->trigger_shutdown(); };
    // Start the pod running
    pod->run();
    return 0;
  #else
    Utils::loglevel = LogLevel::LOG_EDEBUG;
    testing::InitGoogleTest(&argc, argv);  // after calling this, all argc/v related to gtest are removed
    parse_command_line_args(argc, argv, &config_to_open, &flight_plan_to_open);
    podtest_global::config_to_open = config_to_open;
    podtest_global::flight_plan_to_open = flight_plan_to_open;
    return RUN_ALL_TESTS();
  #endif
}

// This is used in tests/PodTest.cpp
// Needed to pass in variables into the testing environment
// Used to pass config file name, specifically
// See Pod.h for where this namespace is declared.
namespace podtest_global {
  std::string config_to_open;
  std::string flight_plan_to_open;
}  // namespace podtest_global
