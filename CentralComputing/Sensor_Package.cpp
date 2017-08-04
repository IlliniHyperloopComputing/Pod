#include "Sensor_Package.h"
#include <iostream>
#include <chrono>

using namespace std;

long long Sensor_Package::start_time = 1;

Sensor_Package::Sensor_Package(vector<Sensor_Configuration> configuration, bool xmega_connect) {

	connect = xmega_connect;
	for(Sensor_Configuration c : configuration){
		Sensor_Group * group;
		switch(c.type){
			case THERMOCOUPLE:
				group = new Thermocouple(c);
				break; //TODO add new sensors here
			case ACCELEROMETERX:
				group = new XAccelerometer(c);
				break;
			case ACCELEROMETERYZ:
				group = new YZAccelerometer(c);
				break;
			case BRAKE_PRESSURE:
				group = new Brake_Pressure(c);
				break;
			case OPTICAL:
				group = new Optical(c);
				break;
			case RIDE_HEIGHT:
				group = new Ride_Height(c);
				break;
			case TAPE_COUNT:
				group = new Tape_Count(c);
				break;
			case BATTERY:
				group = new Battery(c);
				break;
			case CURRENT:
				group = new Current(c);
				break;
			default:
				cout << "Something went wrong creating sensors. " << endl;
				group = NULL;
				break;
					
		}
		sensor_groups[c.type] = group;
	}
	start_time = get_current_time();

  /**
  * 0,1 == X0
  * 2,3 == X1
  * 4,5 == X2
  * 6,7 == Brake
  * 8,9,10,11 == Optical, Delta
  * 12,13,14,15 == Optical, tape count
  **/
  uint8_t bpi1[] = {2,2,2,2,4,4};

  /**
  * 0,1 == y  i2c 
  * 2,3 == z  i2c 
  * 4,5 == RH0 i2c
  * 6,7 == RH1 i2c 
  * 8,9 == RH2 i2c
  * 10,11 == Battery i2c
  * 12,13 == Battery i2c
  * 14,15 == Thermo0 Spi
  * 16,17 == Thermo1
  * 18,19 == Thermo2
  * 20,21 == Thermo3 external
  * 22,23 == Thermo3 internal
  * 24    == RetroReflective  Interrupt
  **/
  uint8_t bpi2[] = {2,2,2,2,2,2,2,2,2,2,2,2,2,1};

  Xmega_Setup x1 = {"/dev/spidev1.0", 6, bpi1, 500000, 8};
  Xmega_Setup x2 = {"/dev/spidev1.1", 14, bpi2, 500000, 8};
	
	if(xmega_connect) {
		spi = new Spi(&x1, &x2);
	} else {
		spi = NULL;
	}
}

Sensor_Package::~Sensor_Package() {
	for(auto const & pair : sensor_groups){
		delete pair.second;
	}
	delete spi;
}

long long Sensor_Package::get_current_time() {

	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void Sensor_Package::update(Xmega_Transfer & transfer) {
	//TODO handle transferring in non simulation cases

	if(connect) {
		spi->transfer(transfer);	
	}
  
	for(auto const & pair : sensor_groups){
		pair.second->update(spi);
	}
}


void Sensor_Package::reset() {
	for(auto const & pair : sensor_groups){
		pair.second->reset();
	}
}

void Sensor_Package::print_status() {
	for(auto const & pair : sensor_groups){
		Sensor_Group * s = pair.second;
		s->print_data();
	}
}

vector<double> Sensor_Package::get_sensor_data(Sensor_Type type) {
	return sensor_groups[type]->get_data();
}

