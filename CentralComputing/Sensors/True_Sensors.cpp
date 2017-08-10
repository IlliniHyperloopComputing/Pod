#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

True_Sensor::True_Sensor(Sensor_Configuration configuration, Sensor_Package * pack) : Sensor_Group(configuration) {
	package = pack; 
	count = 1;
}


True_Sensor::~True_Sensor() {

}


void True_Sensor::reset() {
	
}

void True_Sensor::update(Spi * spi) {
	sensor_group_mutex.lock();
	data[0] = 0;
	sensor_group_mutex.unlock();
}

True_Position::True_Position(Sensor_Configuration configuration, Sensor_Package * pack) : True_Sensor(configuration, pack) { }

True_Velocity::True_Velocity(Sensor_Configuration configuration, Sensor_Package * pack) : True_Sensor(configuration, pack) { }

True_Acceleration::True_Acceleration(Sensor_Configuration configuration, Sensor_Package * pack) : True_Sensor(configuration, pack) { }
void True_Position::update(Spi * spi) {
	vector<double> optical = package->get_sensor_data(OPTICAL);
	vector<double> tape = package->get_sensor_data(TAPE_COUNT);
	sensor_group_mutex.lock();

	//TODO calculate official value
	sensor_group_mutex.unlock();
	
}

void True_Velocity::update(Spi * spi) {

	vector<double> optical = package->get_sensor_data(OPTICAL);
	sensor_group_mutex.lock();

	// TODO: calculate official value
	sensor_group_mutex.unlock();
}

void True_Acceleration::update(Spi * spi) {
	vector<double> accel = package->get_sensor_data(ACCELEROMETERX);
	sensor_group_mutex.lock();

	// TODO: calculate official value

	sensor_group_mutex.unlock();
}

