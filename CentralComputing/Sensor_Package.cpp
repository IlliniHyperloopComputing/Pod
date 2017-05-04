#include "Sensor_Package.h"
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
				break; //TODO add new sensors here
			default:
				cout << "Something went wrong" << endl;
				group = NULL;
				break;
					
		}
		sensor_groups[c.type] = group;
	}
	start_time = get_current_time();

	//TODO set up Spi
	uint8_t bpi1[] = {2,2,2}; //bytes per index for Xmega1
	//16-bit X-acceleration
	//16-bit position
	//16-bit brake pressure
	uint8_t bpi2[] = {2,2,2,2,2,2,2,2, 1, 2,2,2,2, 2,2,2};
	//16-bit Primary Battery Cell1
	//16-bit Primary Battery Cell2
	//16-bit Primary Battery Cell3
	//16-bit Primary Battery Cell4
	//16-bit Secondary Battery Cell1
	//16-bit Secondary Battery Cell2
	//16-bit Secondary Battery Cell3
	//16-bit Secondary Battery Cell4
	//8-bit Number of tapes
	//16-bit Temp1
	//16-bit Temp2
	//16-bit Temp3
	//16-bit Temp4
	//16-bit RideHeight1
	//16-bit RideHeight2
	//16-bit RideHeight3
	Xmega_Setup x1 = {"/dev/spidev1.0", 3, bpi1, 500000, 8};//Xmega1 initialization struct
	Xmega_Setup x2 = {"/dev/spidev1.1",16, bpi2, 500000, 8};

	spi = new Spi(&x1, &x2);
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

void Sensor_Package::update(Xmega_Transfer & transfer) {
	spi->transfer(transfer);	
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
