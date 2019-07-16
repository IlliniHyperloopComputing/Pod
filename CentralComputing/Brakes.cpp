#include "Brakes.h"

Brakes::Brakes() {
  // Do nothing here. At the time the constructor is called,
  // this class can't do anything, since the sensors/ controls are not
  // yet connected
}

void Brakes::enable_brakes() {
  std::lock_guard<std::mutex> guard(mutex);
  #ifdef NO_ACTION
    #ifdef SIM
    SimulatorManager::sim.sim_brake_enable();
    #else 
    print(LogLevel::LOG_INFO, "NO_ACTION: Brakes Enabled\n");
    #endif
  #else
  // Do actually something to enable brakes
  print(LogLevel::LOG_DEBUG, "Brakes Enabled\n");
  #endif
  enabled = true;
}

void Brakes::disable_brakes() {
  std::lock_guard<std::mutex> guard(mutex);
  #ifdef NO_ACTION
    #ifdef SIM
    SimulatorManager::sim.sim_brake_disable();
    #else
    print(LogLevel::LOG_INFO, "NO_ACTION: Brakes Disabled\n"); 
    #endif
  #else
  // Do actually something to disable brakes
  print(LogLevel::LOG_DEBUG, "Brakes Disabled\n");
  #endif

  enabled = false;
}

void Brakes::set_enable(bool enable) {
}

bool Brakes::is_enabled() {
  std::lock_guard<std::mutex> guard(mutex);
  return enabled;
}

