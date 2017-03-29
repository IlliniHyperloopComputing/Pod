#include "Sensor.h"
#include <iostream>

using namespace std;

Sensor_Group::Sensor_Group(Sensor_Configuration configuration) : simulation(configuration.simulation) {
	data = vector<double>(configuration.count);
	//TODO: initialize mutex
}

Sensor_Group::~Sensor_Group(){

}

vector<double> Sensor_Group::get_data() {
	//TODO: lock mutex
	vector<double> d = data;
	//TODO: unlock mutex
	return d;
}

