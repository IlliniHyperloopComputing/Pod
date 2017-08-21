#include "Sensor_Package.h"
#include <iostream>
#include <chrono>

using namespace std;


long long Sensor_Package::start_time = 1;

static uint8_t bpi1_s[] = {2,2,2,2,4,4};
static uint8_t bpi2_s[] = {2,2,2,2,2,2,2,2,2,2,2,2,2,1};
uint8_t * Sensor_Package::bpi1 = bpi1_s;
uint8_t * Sensor_Package::bpi2 = bpi2_s;

Xmega_Setup Sensor_Package::x1 = {"/dev/spidev1.0", 6, bpi1, 500000, 8};
Xmega_Setup Sensor_Package::x2 = {"/dev/spidev1.1", 14, bpi2, 500000, 8};

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
			case TRUE_POSITION:
				group = new True_Position(c, this);
				break;
			case TRUE_VELOCITY:
				group = new True_Velocity(c, this);
				break;
			case TRUE_ACCELERATION:
				group = new True_Acceleration(c, this);
				break;
			case PULL_TAB:
				group = new Pull_Tab(c);
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

	
	if(connect) {
		spi = new Spi(&x1, &x2);

	} else {
		spi = NULL;
	}
}

Sensor_Package::~Sensor_Package() {
	for(auto const & pair : sensor_groups){
		delete pair.second;
	}

  if(connect){
	  delete spi;
  }
}

long long Sensor_Package::get_current_time() {

	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

uint8_t Sensor_Package::update(Xmega_Transfer & transfer) {
	//TODO handle transferring in non simulation cases

	if(connect) {
		uint8_t status = spi->transfer(transfer);	

    //if(status == X_TF_NONE){
      for(auto const & pair : sensor_groups){
        pair.second->update(spi);
      }
    //}

    return status;
	}
  else{
    for(auto const & pair : sensor_groups){
      pair.second->update(spi);
    }
    return 0;
  }
}


void Sensor_Package::reset() {
	for(auto const & pair : sensor_groups){
		pair.second->reset();
	}
}

void Sensor_Package::print_status() {
	
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	cout << "\n----------------Sensor Status at time " << difference << "----------------\n" << endl;
	
	for(auto const & pair : sensor_groups){
		Sensor_Group * s = pair.second;
		s->print_data();
	}
}

uint8_t Sensor_Package::get_sensor_status(uint8_t device){
	if(connect){
    return spi->get_sensor_status(device);
  }
  else{
    return 0;
  }


}

uint8_t Sensor_Package::get_state(uint8_t device){
	if(connect){
    return spi->get_state(device);
  }
  else{
    return 0;
  }
}


vector<double> Sensor_Package::get_sensor_data(Sensor_Type type) {
	return sensor_groups[type]->get_data();
}

size_t Sensor_Package::get_sensor_data_packet_size() {
	size_t size = 0;
	size_t count = 0;
	for(auto & pair : sensor_groups) {
			size += pair.second->get_buffer_size() + 1;
			count += 1;
	}
	size += 11;	//Space for Xmega Responding and Pod State
	return size;
}

uint8_t * Sensor_Package::get_sensor_data_packet() {
	
	uint8_t * buffer = (uint8_t *) malloc(get_sensor_data_packet_size());
	size_t index = 0;
	for(auto & pair : sensor_groups) {
			buffer[index] = pair.first;
			uint8_t * data = pair.second->get_data_buffer();
			size_t data_size = pair.second->get_buffer_size();
			memcpy(buffer + index + 1, data, data_size);
			index += data_size + 1;
			free(data);
	}
	//Xmega State
	memset(buffer + index, 0, 11);
	buffer[index] = XMEGA_STATE;
	buffer[index + 3] = XMEGA_STATUS;
	buffer[index + 6] = XMEGA_RESPONDING;
	buffer[index + 9] = POD_STATE;
	if(connect){
		buffer[index + 1] = spi->get_state(XMEGA1);
		buffer[index + 2] = spi->get_state(XMEGA2);
		buffer[index + 4] = spi->get_sensor_status(XMEGA1);
		buffer[index + 5] = spi->get_sensor_status(XMEGA2);
		
		// TODO: Set up XMEGA Responding buffer
	}

	return buffer;

	
}






