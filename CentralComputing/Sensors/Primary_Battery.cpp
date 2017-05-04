#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Primary_Battery::Primary_Battery(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
}

Primary_Battery::~Primary_Battery(){
	//do nothing
}

void Primary_Battery::reset() {
	//TODO: implement resetting
}

void Primary_Battery::update() {

	switch(simulation) {
		case 0:
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Primary_Battery::simulation_1() {
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
