#include "MotorManager.hpp"
using namespace Utils;
void MotorManager::enable_motors() {
  print(LogLevel::LOG_DEBUG, "Enabling motors\n");
}

void MotorManager::disable_motors() {
  print(LogLevel::LOG_DEBUG, "Disabling motors\n");
}

void MotorManager::set_throttle(uint8_t value) {
  print(LogLevel::LOG_DEBUG, "Setting motor throttle: %d\n", value);
}
