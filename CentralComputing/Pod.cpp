#include "Sensor_Package.h"
#include "Pod.h"
#include "Pod_State.h"
#include <thread>
#include <csignal>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

Sensor_Package * sensors;
Pod_State * state;

volatile bool running = true;

std::tuple<bool, vector<Sensor_Configuration>> configs;

/**
* Gets new sensor values from the XMEGA
*/
void sensor_loop() {
	Xmega_Transfer transfer = {0,X_C_NONE, X_R_ALL};
	while(running){
		
		sensors->update(transfer);
		sensors->print_status();
		usleep(500);
	}

}

void network_loop() {
	//TODO 
  return;
}

void int_handler(int signum) {
  (void)signum;
	running = false;
}


int pod(int argc, char** argv) {
	
	signal(SIGINT, int_handler);
	configs = parse_input(argc, argv);
	state = new Pod_State();
		
	sensors = new Sensor_Package(std::get<1>(configs), std::get<0>(configs));
	printf("Created sensor package\n");
	thread sensor_thread(sensor_loop);
	thread network_thread(network_loop);

	usleep(50000);

	sensor_thread.join();
	network_thread.join();
	delete sensors;	

	return 0;	
} 

std::tuple<bool, vector<Sensor_Configuration>> parse_input(int argc, char** argv) {

	vector<Sensor_Configuration> configs = vector<Sensor_Configuration>();
	
	Sensor_Configuration thermo;
	thermo.type = THERMOCOUPLE;
	thermo.simulation = 0;

	Sensor_Configuration accelx;
	accelx.type = ACCELEROMETERX;
	accelx.simulation = 0;

	Sensor_Configuration accelyz;
	accelyz.type = ACCELEROMETERYZ;
	accelyz.simulation = 0;

	Sensor_Configuration brake;
	brake.type = BRAKE_PRESSURE;
	brake.simulation = 0;

	Sensor_Configuration optical;
	optical.type = OPTICAL;
	optical.simulation = 0;

	Sensor_Configuration height;
	height.type = RIDE_HEIGHT;
	height.simulation = 0;

	Sensor_Configuration tape;
	tape.type = TAPE_COUNT;
	tape.simulation = 0;
	
	Sensor_Configuration battery;
	battery.type = BATTERY;
	battery.simulation = 0;

	Sensor_Configuration current;
	current.type = CURRENT;
	current.simulation = 0;

	size_t simulating_sensors = 0;
	int c;

	while((c = getopt(argc, argv, "a:b:c:h:i:o:t:v:y:"))!= -1){
		switch(c) {
			case 'a':
				accelx.simulation = atoi(optarg);
				break;
			case 'b':
				brake.simulation = atoi(optarg);
				break;
			case 'c':
				tape.simulation = atoi(optarg);
				break;
			case 'h':
				height.simulation = atoi(optarg);
				break;
			case 'i':
				current.simulation = atoi(optarg);
				break;
			case 'o':
				optical.simulation = atoi(optarg);
				break;
			case 't':
				thermo.simulation = atoi(optarg);
				break;
			case 'v':
				battery.simulation = atoi(optarg);
				break;
			case 'y':
				accelyz.simulation = atoi(optarg);
				break;
		}
		simulating_sensors++;
	}

	configs.push_back(thermo);
	configs.push_back(accelx);
	configs.push_back(accelyz);
	configs.push_back(brake);
	configs.push_back(optical);
	configs.push_back(height);
	configs.push_back(tape);
	configs.push_back(battery);
	configs.push_back(current);

	return std::make_tuple(simulating_sensors != NUM_SENSORS, configs);

}

