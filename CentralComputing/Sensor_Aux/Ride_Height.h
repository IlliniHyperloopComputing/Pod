#ifndef RIDE_HEIGHT
#define RIDE_HEIGHT
#include "../Data.h"

struct Ride_Height_Raw {
	double height;
};

struct Ride_Height_Calc {
	double height;
};

Arbitrary_Data ride_height_calculation(Arbitrary_Data raw);

void ride_height_parse(uint8_t * buffer, Arbitrary_Data raw);
#endif
