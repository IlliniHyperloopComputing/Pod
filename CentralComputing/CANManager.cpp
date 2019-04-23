#include "CANManager.h"

bool CANManager::initialize_source() {
  print(LogLevel::LOG_DEBUG, "CAN Manger setup successful\n");
  return true;
}

void CANManager::stop_source() {
  print(LogLevel::LOG_DEBUG, "CAN Manger stopped\n");
}

std::shared_ptr<CANData> CANManager::refresh() {
  std::shared_ptr<CANData> new_data = std::make_shared<CANData>();
  new_data->dummy_data = i;
  i++;
  return new_data;
}

std::shared_ptr<CANData> CANManager::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_can();
  #else
  return empty_data();
  #endif
}

void CANManager::initialize_sensor_error_configs() {
  // From Motor controller 
  // Get Motor Controller over temperature
  // Get Motor over temperature
  // Get DC Link Voltage over voltage
  // Get DC link Voltage under voltage
  // Get Motor Controller Logic Power supply under voltage 
  // Get Motor Controller Logic Power supply over voltage 

  // From BMS
  // Cell Over Voltage
  // Cell under Voltage
  // Over Temperature
  // Battery Under Voltage
  // Battery Over Voltage
  // Over Current

}

void CANManager::check_for_sensor_error(const std::shared_ptr<CANData> & check_data) {
}
