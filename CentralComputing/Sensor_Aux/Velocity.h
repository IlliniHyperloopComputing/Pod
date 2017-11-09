#ifndef VELOCITY_h
#define VELOCITY_h
#include "../Data.h"

struct Velocity_Raw{
	
	int rpm;
};

struct Velocity_Calc{
	double velocity;
};

Arbitrary_Data velocity_calculation(Arbitrary_Data raw);
void velocity_parse(uint8_t * buffer, Arbitrary_Data raw);

#endif // VELOCITY_H

