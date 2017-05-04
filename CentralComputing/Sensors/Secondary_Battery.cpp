#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Secondary_Battery::Secondary_Battery(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
}

Secondary_Battery::~Secondary_Battery(){
	//do nothing
}

void Secondary_Battery::reset() {
	//TODO: implement resetting
}

void Secondary_Battery::update() {

	switch(simulation) {
		case 0:
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Secondary_Battery::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	cout << difference << endl;
	
	//TODO calc some actual value
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}
