#include "Brakes.h"

Brakes::Brakes() {
  // Do nothing here. At the time the constructor is called,
  // this class can't do anything, since the sensors/ controls are not
  // yet connected
}

void Brakes::enable_brakes() {
  #ifdef SIM
  SimulatorManager::sim.sim_brake_enable();
  #else
    #ifdef NO_ACTION
    print(LogLevel::LOG_INFO, "NO_ACTION: Brakes Enabled\n");
    #else
    bool is_brake_set = Utils::set_GPIO(Utils::BRAKE_GPIO, false);
    if (!is_brake_set) {
      Command::set_error_flag(Command::Network_Command_ID::SET_OTHER_ERROR, OTHERErrors::GPIO_SWITCH_ERROR);
    }
    print(LogLevel::LOG_DEBUG, "Brakes Enabled\n");
    #endif
  #endif
  enabled = true;
}

void Brakes::disable_brakes() {
  #ifdef SIM
  SimulatorManager::sim.sim_brake_disable();
  #else
    #ifdef NO_ACTION
    print(LogLevel::LOG_INFO, "NO_ACTION: Brakes Disabled\n"); 
    #else
    // Do actually something to disable brakes
    bool is_brake_set = Utils::set_GPIO(Utils::BRAKE_GPIO, true);
    if (!is_brake_set) {
      Command::set_error_flag(Command::Network_Command_ID::SET_OTHER_ERROR, OTHERErrors::GPIO_SWITCH_ERROR);
    }
    print(LogLevel::LOG_DEBUG, "Brakes Disabled\n");
    #endif
  #endif

  enabled = false;
}

void Brakes::set_enable(bool enable) {
}

bool Brakes::is_enabled() {
  return enabled;
}

