#include "BrakeManager.hpp"

using namespace Utils;

void BrakeManager::activate_brakes() {
  print(LogLevel::LOG_DEBUG, "Activating brakes\n");
  //TODO turn on electromagnet
}

void BrakeManager::deactivate_brakes() {
  print(LogLevel::LOG_DEBUG, "Deactivating brakes\n");
  //TODO release brakes
}


