#ifndef ACCELERATION_Y_H
#define ACCELERATION_Y_H
#include "../Data.h"
#include "../Sensor.h"

struct Acceleration_Y_Raw {
	uint16_t one;
};

void acceleration_y_parse(Spi * spi, Arbitrary_Data raw);
#endif
