#ifndef _POD
#define _POD

#include "Sensor_Package.h"
#include "Pod_State.h"
#include <tuple>

int pod(int argc, char** argv);


std::tuple<bool, vector<Sensor_Configuration>> parse_input(int argc, char** argv);

#endif
