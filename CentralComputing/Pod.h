#ifndef _POD
#define _POD

#include "Sensor_Package.h"
#include "Pod_State.h"
#include <tuple>

int pod(int argc, char** argv);


std::tuple<bool, vector<Sensor_Configuration>> parse_input(int argc, char** argv);

void sensor_loop();
void network_loop();
void read_loop();
void write_loop();


ssize_t read_all_from_socket(int socket, char * buffer, size_t count);
ssize_t write_all_to_socket(int socket, char * buffer, size_t count);
#endif
