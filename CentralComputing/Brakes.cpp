#include "Brakes.h"

Brakes::Brakes() {
  enabled = false;
}

void Brakes::enable_brakes() {
  #ifdef SIM
  SimulatorManager::sim.sim_brake_enable();

  #endif
  enabled = true;
  print(LogLevel::LOG_DEBUG, "Brakes Enabled\n");
}

void Brakes::disable_brakes() {
  #ifdef SIM
  SimulatorManager::sim.sim_brake_disable();

  #endif

  enabled = false;
  print(LogLevel::LOG_DEBUG, "Brakes Disabled\n");
}

void Brakes::set_enable(bool enable) {
}

bool Brakes::is_enabled() {
  return enabled;
}

