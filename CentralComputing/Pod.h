#ifndef POD_H
#define POD_H

#include "Network.h"
#include "Sensor.h"
#include "Utils.h"
#include <thread>
#include <signal.h>
#include <iostream>

void write_loop();
void read_loop();
void network_loop();
void logic_loop();
void int_handler(int signo);
void pipe_handler(int signo);


#endif
