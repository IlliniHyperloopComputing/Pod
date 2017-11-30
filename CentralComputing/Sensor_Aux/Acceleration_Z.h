#ifndef ACCELERATION_Z_H
#define ACCELERATION_Z_H
#include "../Data.h"
#include "../Sensor.h"

struct Acceleration_Z_Raw {
	uint16_t one;
};

void acceleration_z_parse(Spi * spi, Arbitrary_Data raw);
#endif
