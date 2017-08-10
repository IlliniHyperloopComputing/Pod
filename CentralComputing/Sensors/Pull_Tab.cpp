#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Pull_Tab::Pull_Tab(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	count = 1;
	data = vector<double>(count);	
	
	translation_array = {{0.25, 0.25, 0.25, 0.25, 0.0625}};
	name = "Pull_Tab";
	name_array = {{"T1", "T2", "T3", "T4 Ext", "T4 Int"}};
}

Pull_Tab::~Pull_Tab(){
	//do nothing
}

void Pull_Tab::reset() {
	//TODO: implement resetting
}

void Pull_Tab::update(Spi * spi) {

	switch(simulation) {
		case 0:
			// TODO : Read from GPIO pin
			break;
		case 1:
			simulation_1();
			break;
	}
}



void Pull_Tab::simulation_1() {
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
