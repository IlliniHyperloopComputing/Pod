#ifndef ACCELERATION_Y_H
#define ACCELERATION_Y_H
#include "../Data.h"

struct Acceleration_Y_Raw {
	uint16_t one;
};

void acceleration_y_parse(uint8_t * buffer, Arbitrary_Data raw);
#endif
