#include "Sensor.h"
#include <iostream>

using namespace std;

Sensor_Group::Sensor_Group(Sensor_Configuration configuration) : simulation(configuration.simulation) {
}

Sensor_Group::~Sensor_Group(){

}

vector<double> Sensor_Group::get_data() {
	sensor_group_mutex.lock();
	vector<double> d = data;
	sensor_group_mutex.unlock();
	return d;
}

void Sensor_Group::refresh_data(Spi * spi) {
	//lock mutex
	sensor_group_mutex.lock();
	for(int i = 0; i < count; i++) {
		uint32_t val = spi->get_data(device, i + first_index);
		data[i] = val;
	}
	sensor_group_mutex.unlock();
	//unlock 
}
