#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Thermocouple::Thermocouple(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	first_index = THERMOCOUPLE_INDEX;
	device = XMEGA2;
	count = 5;
	data = vector<double>(count);	
	
	translation_array = {{NO_TRANS,NO_TRANS,NO_TRANS,NO_TRANS,NO_TRANS}};
	name = "Thermocouple";
	name_array = {{"T1", "T2", "T3", "T4 Ext", "T4 Int"}};
}

Thermocouple::~Thermocouple(){
	//do nothing
}

void Thermocouple::reset() {
	//TODO: implement resetting
}

void Thermocouple::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);
			break;
		case 1:
			simulation_1();
			break;
	}
}



void Thermocouple::simulation_1() {
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

uint8_t *  Thermocouple::get_data_buffer() {
	sensor_group_mutex.lock();
	uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());
	for(size_t i = 0; i < count; i++){
		uint16_t value = data[i];
		memcpy(buffer + i * sizeof(uint16_t), &value, sizeof(uint16_t));
	}
	sensor_group_mutex.unlock();

	return buffer;
}

size_t Thermocouple::get_buffer_size() {
	// 5 * uint16_t
	return 5 * sizeof(uint16_t);
}
