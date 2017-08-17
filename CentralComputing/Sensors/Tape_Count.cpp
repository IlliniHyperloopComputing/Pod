#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Tape_Count::Tape_Count(Sensor_Configuration configuration) : Sensor_Group(configuration) {
		
	first_index = TAPE_COUNT_INDEX; // index offset to read from spi
	device = XMEGA2; //xmega device number (0 or 1)
	count = 1; //number of sensors
    translation_array = {{1.0}};
    name = "Tape Count";
    name_array = {{"Count"}};
	data = vector<double>(count);
	
}

Tape_Count::~Tape_Count(){
	//do nothing
}

void Tape_Count::reset() {
	//TODO: implement resetting
}

void Tape_Count::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Tape_Count::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	
	//TODO calc some actual value
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}

uint8_t *  Tape_Count::get_data_buffer() {
	sensor_group_mutex.lock();
	uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());
	for(size_t i = 0; i < count; i++){
		uint16_t value = data[i];
		memcpy(buffer + i * sizeof(uint16_t), &value, sizeof(uint16_t));
	}
	sensor_group_mutex.unlock();

	return buffer;
}

size_t Tape_Count::get_buffer_size() {
	// 1 * uint16_t
	return 1 * sizeof(uint16_t);
}
