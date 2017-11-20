#ifndef POD_H
#define POD_H

#include "Network.h"
#include "Sensor.h"
#include "Utils.h"
#include <thread>
#include <iostream>

void write_loop();
void read_loop();
void network_loop();
void xmega_loop();
float pid_controller(int expected_rpm, int actual_rpm);

#endif
