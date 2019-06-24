#include "Motor.h"
using Utils::print;
using Utils::LogLevel;

Motor::Motor() {
  throttle = 0;
  set_motor_state(false);
}

void Motor::enable_motors() {
  set_throttle(MOTOR_OFF);
  set_motor_state(true);
}

void Motor::disable_motors() {
  set_throttle(MOTOR_OFF);
  set_motor_state(false);
}

void Motor::set_motor_state(bool enable) {
  enabled = enable;
  #ifdef NO_ACTION
    #ifdef SIM
    SimulatorManager::sim.sim_motor_state(enable);
    #else
    print(LogLevel::LOG_DEBUG, "NO_ACTION: Motors: %s\n", enable?"Enabled":"Disabled");
    #endif
  #else
  SourceManager::CAN.set_motor_state(enable);
  print(LogLevel::LOG_DEBUG, "Motors: %s\n", enable?"Enabled":"Disabled");
  #endif
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
    #ifdef NO_ACTION
      #ifdef SIM
      SimulatorManager::sim.sim_motor_set_throttle(value);
      #else
      print(LogLevel::LOG_DEBUG, "NO_ACTION: Setting motor throttle: %d\n", value);
      #endif
    #else
    SourceManager::CAN.set_motor_throttle(value);
    print(LogLevel::LOG_DEBUG, "Setting motor throttle: %d\n", value);
    #endif
  }
}

void Motor::set_relay_state(HV_Relay_Select relay, HV_Relay_State state) {
  #ifdef NO_ACTION
    #ifdef SIM
    SimulatorManager::sim.sim_relay_state(relay, state);
    #else
    print(LogLevel::LOG_DEBUG, "NO_ACTION: Relay %d %s\n", relay, state?"Enabled":"Disabled");
    #endif
  #else
  SourceManager::CAN.set_relay_state(relay, state);
  print(LogLevel::LOG_DEBUG, "Relay %d %s\n", relay, state?"Enabled":"Disabled");
  #endif
}
