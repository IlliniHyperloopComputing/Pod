#ifndef VELOCITY_h
#define VELOCITY_h
#include "../Data.h"
#include "../Sensor.h"

struct Velocity_Raw{	
	uint32_t rpm;
};

struct Velocity_Calc{
	double velocity;
};

Arbitrary_Data velocity_calculation(Arbitrary_Data raw);
void velocity_parse(Spi * spi, Arbitrary_Data raw);

#endif // VELOCITY_H

