#include "ADCManager.hpp"

bool ADCManager::initialize_source(){

  print(LogLevel::LOG_DEBUG, "ADC Manger setup successful\n");
  return true;
}

void ADCManager::stop_source(){
  print(LogLevel::LOG_DEBUG, "ADC Manger stopped\n");
}

std::shared_ptr<ADCData> ADCManager::refresh() {

  //this is where you would query the ADC and get new data
  
  std::shared_ptr<ADCData> new_data = std::make_shared<ADCData>();
  new_data->dummy_data = i;
  i++;

  return new_data;
}

std::shared_ptr<ADCData> ADCManager::refresh_sim() {
  return empty_data();
}
