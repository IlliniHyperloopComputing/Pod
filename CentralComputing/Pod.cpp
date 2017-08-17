#include "Sensor_Package.h"
#include "Pod.h"
#include "Pod_State.h"
#include <thread>
#include <csignal>
#include <mutex>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <SafeQueue.hpp>


using namespace std;

Sensor_Package * sensors;
Pod_State * state;
mutex state_mutex;

volatile bool running = true;
int socketfd;
int clientFD;
SafeQueue<Xmega_Command_t> * command_queue;


std::tuple<bool, vector<Sensor_Configuration>> configs;

/**
* Gets new sensor values from the XMEGA
*/
void sensor_loop() {
	Xmega_Transfer transfer = {0,X_C_NONE, X_R_ALL};
	while(running){
		transfer.cmd = command_queue->dequeue();		
		sensors->update(transfer);
		//sensors->print_status();
		usleep(10000);
	}

}

void network_loop() {

	cout << "Server listening on socket " << socketfd << endl;
	struct pollfd p;
	p.fd = socketfd;
	p.events = POLLIN;
	int ret = 0;
	while(running) {
		ret = poll(&p, 1, 1000);
		if(ret == 1) {
			clientFD = accept(socketfd, NULL, NULL);
			if(clientFD > 0){
				cout << "Connected!" << endl;
				thread read_thread(read_loop);
				thread write_thread(write_loop);

				read_thread.join();
				write_thread.join();
				cout << "Waiting for another connection" << endl;
			} else {
				cout << "Accept failed, aborting" << endl;
				break;
			}
		}
	}
	cout << "Network thread exiting!" << endl;
}

void read_loop() {
	cout << "Read thread startup!" << endl;

	char command_buffer = -1;
	while(running && (read(clientFD, &command_buffer, 1) > 0)) {
		cout << "Received: " << (int)command_buffer << endl;
		parse_command(command_buffer);
		command_buffer = -1;
		usleep(10000);
	}

	cout << "Read thread exiting!" << endl;
	
}

void parse_command(char command){
	Command c = (Command)command;
	state_mutex.lock();
	switch(c) {
		case SAFETY_SETUP:
			state->move_safety_setup();
			break;
		case SAFE_MODE:
			state->move_safe_mode();
			break;
		case FUNCTIONAL_TEST:
			state->move_functional_tests();
			break;
		case LAUNCH_READY:
			state->move_launch_ready();
			break;
		case BRAKE:
			state->brake();
			break;
		case EMERGENCY_BRAKE:
			state->emergency_brake();
			break;
		case RESET_SENSORS:
			command_queue->enqueue(X_C_RESET);
			break;
		case CALIBRATE_SENSORS:
			command_queue->enqueue(X_C_CALIBRATE);
			break;	
	}
	state_mutex.unlock();
	cout << "Current state is : " << state->get_current_state_string() << endl;
}

void write_loop() {

	cout << "Write thread startup!" << endl;
	bool active_connection = true;
	while(active_connection && running) {
		usleep(1000000);		
		uint8_t * data = sensors->get_sensor_data_packet();	
		size_t size = sensors->get_sensor_data_packet_size();
		state_mutex.lock();
		data[size-1] = state->get_current_state();
		state_mutex.unlock();
		int result = write_all_to_socket(clientFD, data, size);	
		free(data);
		active_connection = result != -1;
	}
	cout << "Write thread exiting!" << endl;
}

void int_handler(int signum) {
	(void)signum;
	running = false;
	close(socketfd);
	close(clientFD);
	cout << "Closing!" << endl;
}

void pipe_handler(int signum) {
	(void)signum;

}


int pod(int argc, char** argv) {
	struct sigaction a;
	a.sa_handler = int_handler;
	a.sa_flags = 0;
	sigemptyset( &a.sa_mask );
	sigaction( SIGINT, &a, NULL );

	signal(SIGPIPE, pipe_handler);
	configs = parse_input(argc, argv);
	state = new Pod_State();	
	sensors = new Sensor_Package(std::get<1>(configs), std::get<0>(configs));
	command_queue = new SafeQueue<Xmega_Command_t>();
	printf("Created sensor package\n");
	thread sensor_thread(sensor_loop);

	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	struct addrinfo hints, *result;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int s = getaddrinfo(NULL, "8800", &hints, &result);
	if(s != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(1);
	}
	if(bind(socketfd, result->ai_addr, result->ai_addrlen) != 0){
		perror("bind");
		exit(1);
	}
	if(listen(socketfd, 1) != 0){
		perror("listen");
		exit(1);
	}
	free(result);

	cout << "Starting network thread" << endl;
	thread network_thread(network_loop);


	sensor_thread.join();
	network_thread.join();
	delete sensors;	
	delete state;
	delete command_queue;

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

	Sensor_Configuration true_acceleration;
	true_acceleration.type = TRUE_ACCELERATION;
	true_acceleration.simulation = 0;

	Sensor_Configuration true_velocity;
	true_velocity.type = TRUE_VELOCITY;
	true_velocity.simulation = 0;

	Sensor_Configuration true_position;
	true_position.type = TRUE_POSITION;
	true_position.simulation = 0;

	Sensor_Configuration pull_tab;
	pull_tab.type = PULL_TAB;
	pull_tab.simulation = 0;

	size_t simulating_sensors = 0;
	int c;

	while((c = getopt(argc, argv, "a:b:c:h:i:o:t:v:y:p:"))!= -1){
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
			case 'p':
				pull_tab.simulation = atoi(optarg);
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
	configs.push_back(true_position);
	configs.push_back(true_velocity);
	configs.push_back(true_acceleration);
	configs.push_back(pull_tab);



	return std::make_tuple(simulating_sensors != NUM_SENSORS, configs);

}

ssize_t read_all_from_socket(int socket, char *buffer, size_t count) {
	size_t bytes_read = 0;
	while(bytes_read != count){
		int bytes = read(socket, buffer + bytes_read, count - bytes_read);
		if(bytes > 0)
			bytes_read += bytes;
		else if(bytes == 0){
			fprintf(stderr, "Disconnected\n");
			return 0;
		} else if(bytes == -1 && errno != EINTR){
			fprintf(stderr, "Failure\n");
			return -1;
		}
	}
	return bytes_read;
}

ssize_t write_all_to_socket(int socket, uint8_t *buffer, size_t count) {
	size_t bytes_written = 0;
	while(bytes_written != count){
		//fprintf(stderr,"Writing to socket\n");
		int bytes = write(socket, buffer + bytes_written, count - bytes_written);
		if(bytes > 0)
			bytes_written += bytes;
		else if(bytes == 0){
			fprintf(stderr, "Disconnected\n");
			return 0;
		} else if(bytes == -1 && errno != EINTR){
			fprintf(stderr, "Write failure!\n");
			return -1;
		}
	}
	return bytes_written;
}

