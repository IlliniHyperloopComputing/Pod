#include "Sensor.h"
#include "Utils.h"
#include <iostream>

using namespace std;

Sensor_Group::Sensor_Group(Sensor_Configuration configuration) : simulation(configuration.simulation) {
}

Sensor_Group::~Sensor_Group(){

}

vector<double> Sensor_Group::get_data() {
	sensor_group_mutex.lock();
	vector<double> d = data;
	sensor_group_mutex.unlock();
	return d;
}

void Sensor_Group::refresh_data(Spi * spi) {
	//lock mutex
	sensor_group_mutex.lock();
	for(size_t i = 0; i < count; i++) {
    print_debug("spi->get_data()\n");
		uint32_t val = spi->get_data(device, i + first_index);
		data[i] = translation_array[i](val);
	}
	sensor_group_mutex.unlock();
	//unlock 
}

void Sensor_Group::print_data() {
  cout << "\t" << name << ": " << data.size() << " sensors" << endl;
  for(uint32_t i = 0; i < data.size(); i++){
	cout << "\t" << name_array[i] << ":\t"<< data[i] << endl;
  }
}

double Sensor_Group::no_trans(double x){
  return x;
}
double Sensor_Group::adc_trans(double x){
  return (x/32769.0 * 4.096);
}

double Sensor_Group::current_trans(double x){
  return (x * 0.1);
}

double Sensor_Group::rpm_trans(double x){
  return 1.0/(x / 32768.0);
}

double Sensor_Group::thermo_ext_trans(double x){
  return x * 0.25;
}

double Sensor_Group::thermo_int_trans(double x){
  return x * 0.0625;
}
