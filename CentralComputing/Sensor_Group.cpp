#include "Sensor.h"
#include <iostream>

using namespace std;

Sensor_Group::Sensor_Group(Sensor_Configuration configuration) : simulation(configuration.simulation) {
	data = vector<double>(configuration.count);
	
}

Sensor_Group::~Sensor_Group(){

}

vector<double> Sensor_Group::get_data() {
	sensor_group_mutex.lock();
	vector<double> d = data;
	sensor_group_mutex.unlock();
	return d;
}

