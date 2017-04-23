#include "Sensor_Package.h"
#include <iostream>

using namespace std;

Thermocouple::Thermocouple(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
}

Thermocouple::~Thermocouple(){
	//do nothing
}

void Thermocouple::reset() {
	//TODO: implement resetting
}

void Thermocouple::update(const Spi* spi) {

	switch(simulation) {
		case 0:
			spi->get_data(device, idx);
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
	cout << difference << endl;
	
	//TODO calc some actual value
	
	//Creates a mutex lock that automatically unlocks after it falls out of scope
	//Cleaner than calling mutex lock
	lock_guard<mutex> guard(sensor_group_mutex);
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
}
