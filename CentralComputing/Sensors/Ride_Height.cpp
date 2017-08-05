#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Ride_Height::Ride_Height(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	
	first_index = RIDE_HEIGHT_INDEX; // index offset to read from spi
	device = XMEGA2; //xmega device number (0 or 1)
	count = 3; //number of sensors
    translation_array = {{ADC_TRANS,ADC_TRANS,ADC_TRANS}};
    name = "Ride Height";
    name_array = {{"RH1", "RH2", "RH3"}};
	data = vector<double>(count);		
}

Ride_Height::~Ride_Height(){
	//do nothing
}

void Ride_Height::reset() {
	//TODO: implement resetting
}

void Ride_Height::update(Spi * spi) {

	switch(simulation) {
		case 0:
			refresh_data(spi);
			break;
		case 1:
			simulation_1();
			break;
	}
}

void Ride_Height::simulation_1() {
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
