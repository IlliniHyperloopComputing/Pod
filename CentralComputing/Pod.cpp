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
volatile bool send_manual_brake = false;

int consecutive_errors_1 = 0;
int consecutive_errors_2 = 0;


/**
* Gets new sensor values from the XMEGA
*/
void sensor_loop() {
  int i = 0;
	Xmega_Transfer transfer = {0,X_C_NONE, X_R_ALL};
	while(running){

    //Xmega1
	  transfer.device = 0;
    if(send_manual_brake){
      transfer.cmd = X_C_MANUAL_BRAKE;
      send_manual_brake = 0;
    }
    else{
      transfer.cmd = X_C_NONE;
    }
    uint8_t stat = sensors->update(transfer);
    if(stat != X_TF_NONE){
      consecutive_errors_1++;
    }
    else{
      consecutive_errors_1 = 0;
    }

    //Sleep for 25 milliseconds
    usleep(25000);

    //Xmega2
    transfer.device = 1;
    transfer.cmd = X_C_NONE;
    sensors->update(transfer);
    if(stat != X_TF_NONE){
      consecutive_errors_2++;
    }
    else{
      consecutive_errors_2 = 0;
    }


    if(i % 5 == 0){
      cout << "=============================="<< endl;
      sensors->print_status();
      printf("Xmega1 State: %d \t Xmega2 State: %d \n", sensors->get_state(0), sensors->get_state(1));
      printf("Xmega1 Sensor_Status: %x Xmega2 Sensor_Status: %x\n", sensors->get_sensor_status(0), sensors->get_sensor_status(1));
      printf("Consecutive1: %d \t Consecutive2: %d\n", consecutive_errors_1, consecutive_errors_2);
    }

    i++;
	}

}

void network_loop() {
	//TODO 
  return;
}

void int_handler(int signum) {
  (void)signum;
  running = false;
  return;
}

//Send SIGUSR1 (10, 16, 30)
void cmd_handler(int signum) {
  (void)signum;
  send_manual_brake = 1;
}

int pod(int argc, char** argv) {
    
  struct sigaction act;
  memset(&act, 0, sizeof(struct sigaction));

  act.sa_handler = &int_handler;
  sigaction(SIGINT, &act, NULL);

  act.sa_handler = &cmd_handler;
  sigaction(SIGUSR1, &act, NULL);

	auto configs = parse_input(argc, argv);
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

