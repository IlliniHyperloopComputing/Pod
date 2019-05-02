#include "I2CManager.h"

bool I2CManager::initialize_source() {
  print(LogLevel::LOG_DEBUG, "I2C Manger setup successful\n");
  return true;
}

void I2CManager::stop_source() {
  print(LogLevel::LOG_DEBUG, "I2C Manger stopped\n");
}

std::shared_ptr<I2CData> I2CManager::refresh() {
  std::shared_ptr<I2CData> new_data = std::make_shared<I2CData>();
  new_data->dummy_data = i;
  i++;
  return new_data;
}

std::shared_ptr<I2CData> I2CManager::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_i2c();
  #else
  return empty_data();
  #endif
}

void I2CManager::initialize_sensor_error_configs() {
  if (!(ConfiguratorManager::config.getValue("error_general_1_over_temp", error_general_1_over_temp) && 
      ConfiguratorManager::config.getValue("error_general_2_over_temp",   error_general_2_over_temp) &&
      ConfiguratorManager::config.getValue("error_general_3_over_temp",   error_general_3_over_temp))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: I2CManager Missing necessary configuration\n");
    exit(1);
  }
  // Over Temperature conditionns
  // Possibly different thresholds for each measurement??
}

void I2CManager::check_for_sensor_error(const std::shared_ptr<I2CData> & check_data) {
}
