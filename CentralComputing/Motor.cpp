#include "Motor.h"
using Utils::print;
using Utils::LogLevel;

Motor::Motor() {
  // Do nothing here. At the time the constructor is called,
  // the motor class can't do anything, since the sensors/ controls are not
  // yet connected
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
  std::lock_guard<std::mutex> guard(mutex);
  enabled = enable;
  #ifdef NO_ACTION
    #ifdef SIM
    SimulatorManager::sim.sim_motor_state(enable);
    #else
    print(LogLevel::LOG_INFO, "NO_ACTION: Motors: %s\n", enable?"Enabled":"Disabled");
    #endif
  #else
  SourceManager::CAN.set_motor_state(enable);
  print(LogLevel::LOG_INFO, "Motors: %s\n", enable?"Enabled":"Disabled");
  #endif
}

bool Motor::is_enabled() {
  std::lock_guard<std::mutex> guard(mutex);
  return enabled;
}

int16_t Motor::get_throttle() {
  std::lock_guard<std::mutex> guard(mutex);
  return throttle;
}

void Motor::set_throttle(int16_t value) {
  std::lock_guard<std::mutex> guard(mutex);
  if (enabled) {
    throttle = value;
    #ifdef NO_ACTION
      #ifdef SIM
      SimulatorManager::sim.sim_motor_set_throttle(value);
      #else
      print(LogLevel::LOG_INFO, "NO_ACTION: Setting motor throttle: %d\n", value);
      #endif
    #else
    SourceManager::CAN.set_motor_throttle(value);
    print(LogLevel::LOG_INFO, "Setting motor throttle: %d\n", value);
    #endif
  }
}

void Motor::set_relay_state(HV_Relay_Select relay, HV_Relay_State state) {
  std::lock_guard<std::mutex> guard(mutex);
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
