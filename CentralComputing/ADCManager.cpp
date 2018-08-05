#include "ADCManager.hpp"

bool ADCManager::initialize_source(){

  return true;
}

std::shared_ptr<ADCData> ADCManager::refresh() {

  //this is where you would query the ADC and get new data
  
  std::shared_ptr<ADCData> new_data = std::make_shared<ADCData>();
  new_data->dummy_data = i;
  i++;
  return new_data;
}

