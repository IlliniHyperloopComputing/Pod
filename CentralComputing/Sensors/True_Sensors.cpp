#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

True_Sensor::True_Sensor(Sensor_Configuration configuration, Sensor_Package * pack) : Sensor_Group(configuration) {
	package = pack; 
	count = 1;
	data = vector<double>(count);	
  translation_array = {{NO_TRANS}};
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

True_Position::True_Position(Sensor_Configuration configuration, Sensor_Package * pack) : True_Sensor(configuration, pack) {
	name = "True Position";
	name_array = {{"Position"}};
}

True_Velocity::True_Velocity(Sensor_Configuration configuration, Sensor_Package * pack) : True_Sensor(configuration, pack) { 
	
	name = "True Velocity";
	name_array = {{"Velocity"}};
}

True_Acceleration::True_Acceleration(Sensor_Configuration configuration, Sensor_Package * pack) : True_Sensor(configuration, pack) { 

	name = "True Acceleration";
	name_array = {{"Acceleration"}};
}

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
  (void) spi;
	vector<double> accel = package->get_sensor_data(ACCELEROMETERX);
	sensor_group_mutex.lock();

  double true_accel = 0;

  //Official Acceleration is simply the median of the three values
  if(accel[0] > accel[1]){
    if(accel[1] > accel[2]){
      true_accel = accel[1];
    }
    else if(accel[0] > accel[2]){
      true_accel = accel[2];
    }
    else{
      true_accel = accel[0];
    }
  }
  else{
    if(accel[0] > accel[2]){
      true_accel = accel[0];
    }
    else if(accel[1] > accel[2]){
      true_accel = accel[2];
    }
    else{
      true_accel = accel[1];
    }
  }

  data[0] = true_accel;

	sensor_group_mutex.unlock();
}

uint8_t *  True_Sensor::get_data_buffer() {
	sensor_group_mutex.lock();
	uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());
	memcpy(buffer, &data[0], sizeof(double));
	sensor_group_mutex.unlock();
	return buffer;
}

size_t True_Sensor::get_buffer_size() {
	return sizeof(double);
}
