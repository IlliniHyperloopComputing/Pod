#ifndef ACCELERATION_X_H
#define ACCELERATION_X_H
#include "../Data.h"
#include "../Sensor.h"

struct Acceleration_X_Raw {
	uint16_t one;
	uint16_t two;
	uint16_t three;
	bool calibrated;
	double calibrated_baseline;
	int current_samples;
	int total_samples;
};

struct Acceleration_X_Calc {
	double x;
};

Arbitrary_Data acceleration_x_calculation(Arbitrary_Data raw);
void acceleration_x_parse(Spi * spi, Arbitrary_Data raw); 
#endif
