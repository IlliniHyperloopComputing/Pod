#ifndef VOLTAGE_h
#define VOLTAGE_h
#include "../Data.h"
#include "../Sensor.h"

struct Voltage_Raw{	
	uint16_t voltage
};

struct Voltage_Calc{
	double voltage;
};

Arbitrary_Data voltage_calculation(Arbitrary_Data raw);
void voltage_parse(Spi * spi, Arbitrary_Data raw);

#endif // VELOCITY_H