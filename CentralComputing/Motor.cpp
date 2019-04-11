#include "Motor.h"
using Utils::print;
using Utils::LogLevel;

Motor::Motor() {
  // TODO: Setup CAN connection here
  throttle = 0;
  set_enable(false);
}

void Motor::enable_motors() {
  set_throttle(MOTOR_OFF);
  set_enable(true);

  #ifdef SIM
  SimulatorManager::sim.sim_motor_enable();
  #endif

  print(LogLevel::LOG_DEBUG, "Motors Enabled\n");
}

void Motor::disable_motors() {
  set_throttle(MOTOR_OFF);
  set_enable(false);


  #ifdef SIM
  SimulatorManager::sim.sim_motor_disable();
  #endif

  print(LogLevel::LOG_DEBUG, "Motors Disabled\n");
}

void Motor::set_enable(bool enable) {
  // TODO: Something over CAN
  enabled = enable;
}

bool Motor::is_enabled() {
  return enabled;
}

int16_t Motor::get_throttle() {
  return throttle;
}

void Motor::set_throttle(int16_t value) {
  if (enabled) {
    throttle = value;

    // TODO: Something over CAN

    #ifdef SIM
    SimulatorManager::sim.sim_motor_set_throttle(value);
    #endif
    
    print(LogLevel::LOG_DEBUG, "Setting motor throttle: %d\n", value);
  }
}
