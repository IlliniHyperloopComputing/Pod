#ifndef TACHOMETER_H
#define TACHOMETER_H
#include "../Data.h"

struct Tachometer_Raw{
	int rpm;
};

struct Tachometer_Calc{
	double rpm;
};

Arbitrary_Data tachometer_calculation(Arbitrary_Data raw);
void tachometer_parse(uint8_t * buffer, Arbitrary_Data raw);

#endif // TACHOMETER_H
