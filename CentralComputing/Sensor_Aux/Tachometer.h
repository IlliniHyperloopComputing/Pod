Tachometer.h
#ifndef TACHOMETER_h
#define TACHOMETER_h
#include "../Data.h"

struct Tachometer_Raw{
	
	int rpm;
};

struct Tachometer_Calc{
	double tachometer;
};

Arbitrary_Data tachometer_calculation(Arbitrary_Data raw);
void tachometer_parse(uint8_t * buffer, Arbitrary_Data raw);

#endif // TACHOMETER_H
