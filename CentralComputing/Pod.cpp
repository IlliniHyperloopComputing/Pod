#include "Sensor_Package.h"

#include <thread>

using namespace std;

Sensor_Package * sensors;
/**
* Gets new sensor values from the XMEGA
*/
void sensor_loop() {
	//TODO construct XMEGA transfer object
	//sensors->update();
}

int pod(int argc, char** argv) {
	
	vector<Sensor_Configuration> configs = vector<Sensor_Configuration>();

	//sets up thermocouple configuration
	Sensor_Configuration thermo;
	thermo.type = THERMOCOUPLE;
	thermo.simulation = 0;
	thermo.count = 4;

	//TODO set up other configurations
	
	sensors = new Sensor_Package(configs);
	thread sensor_thread(sensor_loop);
	


	return 0;
	
}
