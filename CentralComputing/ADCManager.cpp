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

    std::string fileName = "/dev/iio:device0";

    ifstream inFile;
    inFile.open(fileName, std::ifstream::in | std::ifstream::binary);
    if (!inFile) {
	return empty_data();
    }
    std::shared_ptr<ADCData> new_data = std::make_shared<ADCData>();
    uint16_t val;
    for (int i = 0; i < 7; i++) {
	inFile.read(&val, 2);
	new_data -> accel[i] = val/4096 * 1.8;
    }


  return new_data;
}

std::shared_ptr<ADCData> ADCManager::refresh_sim() {
  return empty_data();
}
