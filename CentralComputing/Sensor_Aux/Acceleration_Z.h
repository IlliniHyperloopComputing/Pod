#ifndef ACCELERATION_Z_H
#define ACCELERATION_Z_H
#include "../Data.h"

struct Acceleration_Z_Raw {
	uint16_t one;
};

void acceleration_z_parse(uint8_t * buffer, Arbitrary_Data raw);
#endif
