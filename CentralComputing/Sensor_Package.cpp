#include "Sensor.h"
#include <iostream>
#include <chrono>

using namespace std;
long long Sensor_Package::start_time = 1;
Sensor_Package::Sensor_Package(vector<Sensor_Configuration> configuration) {
	for(Sensor_Configuration c : configuration){
		Sensor_Group * group;
		switch(c.type){
			case THERMOCOUPLE:
				group = new Thermocouple(c);
				break;
			default:
				cout << "Something went wrong" << endl;
				group = NULL;
				break;
					
		}
		sensor_groups[c.type] = group;
	}
	start_time = get_current_time();
}

Sensor_Package::~Sensor_Package() {
	for(auto const & pair : sensor_groups){
		delete pair.second;
	}
}

long long Sensor_Package::get_current_time() {

	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void Sensor_Package::update() {
	for(auto const & pair : sensor_groups){
		pair.second->update();
	}
}


void Sensor_Package::reset() {
	for(auto const & pair : sensor_groups){
		pair.second->reset();
	}
}

vector<double> Sensor_Package::get_sensor_data(Sensor_Type type) {
	return sensor_groups[type]->get_data();
}
