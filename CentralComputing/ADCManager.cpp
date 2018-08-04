#include "ADCManager.hpp"

std::shared_ptr<ADCData> ADCManager::refresh() {

  //this is where you would query the ADC and get new data
  
  std::shared_ptr<ADCData> new_data = std::make_shared<ADCData>();
  new_data->dummy_data = i;
  i++;
  return new_data;
}

