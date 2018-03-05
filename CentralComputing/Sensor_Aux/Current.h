#ifndef CURRENT_H
#define CURRENT_H
#include "../Data.h"
#include "../Sensor.h"

struct Current_Raw {
	uint16_t current;
};

void current_parse(void * source, Arbitrary_Data raw);
#endif
