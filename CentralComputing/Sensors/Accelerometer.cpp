#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Accelerometer::Accelerometer(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
}

Accelerometer::~Accelerometer(){
	//do nothing
}

void Accelerometer::reset() {
	//TODO: implement resetting
}

void Accelerometer::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);	
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Accelerometer::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	cout << difference << endl;
	
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}
