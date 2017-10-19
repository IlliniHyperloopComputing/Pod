#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "../Data.h"

struct Temperature_Raw {
	uint8_t temperature_brakes;
	uint8_t temperature_batteries;
	uint8_t temperature_cpu;
	uint8_t temperature_motor;
	uint8_t temperature_wheels;
}

void temperature_parse(uint8_t * buffer, Arbitrary_Data raw);

#endif
