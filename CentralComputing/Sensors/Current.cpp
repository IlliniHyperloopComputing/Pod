#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Current::Current(Sensor_Configuration configuration) : Sensor_Group(configuration) {

	first_index = CURRENT_INDEX; // index offset to read from spi
	device = XMEGA2; //xmega device number (0 or 1)
	count = 2; //number of sensors
  translation_array = {{NO_TRANS, NO_TRANS}};
  name = "Current";
  name_array = {{"C1","C2"}};
	data = vector<double>(count);		
}

Current::~Current(){
	//do nothing
}

void Current::reset() {
	//TODO: implement resetting
}

void Current::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);	
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Current::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}

uint8_t *  Current::get_data_buffer() {
	uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());
	sensor_group_mutex.lock();
	for(size_t i = 0; i < count; i++){
		uint16_t value = data[i];
		memcpy(buffer + i * sizeof(uint16_t), &value, sizeof(uint16_t));
	}
	sensor_group_mutex.unlock();

	return buffer;
}

size_t Current::get_buffer_size() {
	// 2 * uint16_t
	return 2 * sizeof(uint16_t);
}