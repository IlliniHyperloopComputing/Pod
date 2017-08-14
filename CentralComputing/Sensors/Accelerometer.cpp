#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////
//X
XAccelerometer::XAccelerometer(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
	first_index = X_ACCELERATION_INDEX; // index offset to read from spi
	device = XMEGA1; //xmega device number (0 or 1)
	count = 3; //number of sensors
	data = vector<double>(count);
    translation_array = {{ADC_TRANS,ADC_TRANS,ADC_TRANS}};
    name = "X Accel";
    name_array = {{"X1", "X2", "X3"}};
}

XAccelerometer::~XAccelerometer(){
	//do nothing
}

void XAccelerometer::reset() {
	//TODO: implement resetting
}

void XAccelerometer::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);	
			break;
		case 1:
			simulation_1();
			break;
	}
}

void XAccelerometer::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}

uint8_t *  XAccelerometer::get_data_buffer() {
	uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());
	for(size_t i = 0; i < count; i++){
		uint16_t value = data[i];
		memcpy(buffer + i * sizeof(uint16_t), &value, sizeof(uint16_t));
	}

	return buffer;
}

size_t XAccelerometer::get_buffer_size() {
	// 3 * uint16_t
	return 3 * sizeof(uint16_t);
}


///////////////////////////////////////////////////////////////
//YZ

YZAccelerometer::YZAccelerometer(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
	first_index = YZ_ACCELERATION_INDEX; // index offset to read from spi
	device = XMEGA2; //xmega device number (0 or 1)
	count = 2; //number of sensors
    translation_array = {{ADC_TRANS,ADC_TRANS,ADC_TRANS}};
    name = "YZ Accel";
    name_array = {{"Y", "Z"}};
	data = vector<double>(count);
}

YZAccelerometer::~YZAccelerometer(){
	//do nothing
}

void YZAccelerometer::reset() {
	//TODO: implement resetting
}

void YZAccelerometer::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);	
			break;
		case 1:
			simulation_1();
			break;
	}
}

void YZAccelerometer::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}

uint8_t *  YZAccelerometer::get_data_buffer() {
	uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());
	for(size_t i = 0; i < count; i++){
		uint16_t value = data[i];
		memcpy(buffer + i * sizeof(uint16_t), &value, sizeof(uint16_t));
	}

	return buffer;
}

size_t YZAccelerometer::get_buffer_size() {
	// 2 * uint16_t
	return 2 * sizeof(uint16_t);
}
