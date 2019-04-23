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

}

void ADCManager::check_for_sensor_error(const std::shared_ptr<ADCData> & check_data) {
}
