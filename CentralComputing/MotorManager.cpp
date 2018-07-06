#include "MotorManager.hpp"

void MotorManager::enable_motors() {
  print_info("Enabling motors\n");
}

void MotorManager::disable_motors() {
  print_info("Disabling motors\n");
}

void MotorManager::set_throttle(uint8_t value) {
  print_info("Setting motor throttle: %d\n", value);
}
