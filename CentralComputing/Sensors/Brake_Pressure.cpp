#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Brake_Pressure::Brake_Pressure(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
	first_index = BRAKE_PRESSURE_INDEX; // index offset to read from spi
	device = XMEGA1; //xmega device number (0 or 1)
	count = 1; //number of sensors
    translation_array = {{ADC_TRANS}};
    name = "Brake Pressure";
    name_array = {{"Pressure"}};
	data = vector<double>(count);		
}

Brake_Pressure::~Brake_Pressure(){
	//do nothing
}

void Brake_Pressure::reset() {
	//TODO: implement resetting
}

void Brake_Pressure::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Brake_Pressure::simulation_1() {
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
