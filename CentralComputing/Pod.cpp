#include "Sensor_Package.h"
#include "Pod.h"

#include <thread>
#include <csignal>

using namespace std;

Sensor_Package * sensors;
bool running = true;

/**
* Gets new sensor values from the XMEGA
*/
void sensor_loop() {
	//TODO construct XMEGA transfer object
	Xmega_Transfer transfer;
	while(running) {
		sensors->update(transfer);
	}

}

void int_handler(int signum) {
	running = false;
}


int pod(int argc, char** argv) {
	
	signal(SIGINT, int_handler);
	vector<Sensor_Configuration> configs = vector<Sensor_Configuration>();

	//sets up thermocouple configuration
	Sensor_Configuration thermo;
	thermo.type = THERMOCOUPLE;
	thermo.simulation = 1;
	configs.push_back(thermo);

/*
	Sensor_Configuration accel;
	accel.type = ACCELEROMETER;
	accel.simulation = 1;
	configs.push_back(accel);
	*/

	//TODO set up other configurations

	
	sensors = new Sensor_Package(configs, false);
	printf("Created sensor package\n");
	thread sensor_thread(sensor_loop);

	usleep(50000);
	raise(SIGINT);

	sensor_thread.join();
	delete sensors;	


	return 0;
	
}
