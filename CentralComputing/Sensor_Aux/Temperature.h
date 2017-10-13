#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "../Data.h"

struct Temperature_Raw {
	uint8_t temperature_ext;
	uint8_t temperature_int;
}

struct Temperature_Calc {
	double temperature_ext;
	double temperature_int;
}

Arbitrary_Data temperature_calculation(Arbitrary_Data raw);
void temperature_parse(uint8_t * buffer, Arbitrary_Data raw);

#endif
