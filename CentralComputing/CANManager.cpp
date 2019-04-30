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
  if (!(ConfiguratorManager::config.getValue("error_motor_ctrl_over_temp", error_motor_ctrl_over_temp) && 
      ConfiguratorManager::config.getValue("error_motor_over_temp", error_motor_over_temp) &&
      ConfiguratorManager::config.getValue("error_dc_link_over_voltage", error_dc_link_over_voltage) &&
      ConfiguratorManager::config.getValue("error_dc_link_under_voltage", error_dc_link_under_voltage) &&
      ConfiguratorManager::config.getValue("error_motor_ctrl_logic_over_voltage",  error_motor_ctrl_logic_over_voltage) &&  // NOLINT
      ConfiguratorManager::config.getValue("error_motor_ctrl_logic_under_voltage", error_motor_ctrl_logic_under_voltage) && // NOLINT
      ConfiguratorManager::config.getValue("error_cell_over_voltage",  error_cell_over_voltage) &&
      ConfiguratorManager::config.getValue("error_cell_under_voltage", error_cell_under_voltage) &&
      ConfiguratorManager::config.getValue("error_cell_over_temp", error_cell_over_temp) &&
      ConfiguratorManager::config.getValue("error_battery_over_voltage",  error_battery_over_voltage) &&
      ConfiguratorManager::config.getValue("error_battery_under_voltage", error_battery_under_voltage) &&
      ConfiguratorManager::config.getValue("error_battery_over_current", error_battery_over_current))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: CANManager Missing necessary configuration\n");
    exit(1);
  }
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
  // Grab all configuration variables
}

void CANManager::check_for_sensor_error(const std::shared_ptr<CANData> & check_data) {
}
