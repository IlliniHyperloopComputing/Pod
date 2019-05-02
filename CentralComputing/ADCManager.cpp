#include "ADCManager.h"

bool ADCManager::initialize_source() {
  print(LogLevel::LOG_DEBUG, "ADC Manger setup successful\n");
  return true;
}

void ADCManager::stop_source() {
  print(LogLevel::LOG_DEBUG, "ADC Manger stopped\n");
}

std::shared_ptr<ADCData> ADCManager::refresh() {
  // this is where you would query the ADC and get new data
  
  std::shared_ptr<ADCData> new_data = std::make_shared<ADCData>();
  new_data->dummy_data = i;
  i++;

  return new_data;
}

std::shared_ptr<ADCData> ADCManager::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_adc();
  #else
  return empty_data();
  #endif
}

void ADCManager::initialize_sensor_error_configs() {
  if (!(ConfiguratorManager::config.getValue("error_accel_diff", error_accel_diff) && 
      ConfiguratorManager::config.getValue("error_pneumatic_1_over_pressure", error_pneumatic_1_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_2_over_pressure", error_pneumatic_2_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_3_over_pressure", error_pneumatic_3_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_4_over_pressure", error_pneumatic_4_over_pressure) &&
      ConfiguratorManager::config.getValue("error_battery_box_over_pressure",  error_battery_box_over_pressure) &&
      ConfiguratorManager::config.getValue("error_battery_box_under_pressure", error_battery_box_under_pressure))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: ADCManager Missing necessary configuration\n");
    exit(1);
  }
  // For accelerometers
  // Get Standard Deviation to check against (or just the difference)

  // For other measurements
  // Pneumatic pressure over pressure 1
  // Pneumatic pressure over pressure 2
  // Pneumatic pressure over pressure 3
  // Pneumatic pressure over pressure 4
  // Battery Box over pressure
  // Battery Box under pressure
}

void ADCManager::check_for_sensor_error(const std::shared_ptr<ADCData> & check_data) {
}
