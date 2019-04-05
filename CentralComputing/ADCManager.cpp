#include "ADCManager.hpp"


bool ADCManager::initialize_source(){

    fileName = "/dev/iio:device0";
    inFile.open(fileName, std::ifstream::in | std::ifstream::binary);
    if (!inFile) {
	print(LogLevel::LOG_DEBUG, "ADC Manager setup failed\n");
    } else {
	print(LogLevel::LOG_DEBUG, "ADC Manager setup successful\n");
	return true;
    }
}

void ADCManager::stop_source(){
    if (inFile) {
	inFile.close();
	print(LogLevel::LOG_DEBUG, "ADC Manager stopped\n");
    } else {
	print(LogLevel::LOG_DEBUG, "ADC Manager never opened\n");
    }
}

std::shared_ptr<ADCData> ADCManager::refresh() {

  //this is where you would query the ADC and get new data

    std::shared_ptr<ADCData> new_data = std::make_shared<ADCData>();
    uint8_t buffer [NUM_INPUTS * 2];
    inFile.read(buffer, NUM_INPUTS * 2);
    for (int i = 0; i < NUM_INPUTS; i++) {
	uint16_t * val = (uint16_t * )(buffer + 2 * i);
	new_data -> accel[i] = (*val) * 4096 / 1.8;
    }
    return new_data;
}

std::shared_ptr<ADCData> ADCManager::refresh_sim() {
  return empty_data();
}
