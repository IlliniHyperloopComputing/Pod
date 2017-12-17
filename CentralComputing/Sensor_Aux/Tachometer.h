Tachometer.h
#ifndef TACHOMETER_h
#define TACHOMETER_h
#include "../Data.h"

struct Tachometer_Raw{
	
	int rpm;
	uint32_t left;
  	uint32_t middle;
  	uint32_t right; 
  	uint32_t lfront; 
  	uint32_t rfront; 
  	uint32_t back; 
};

struct Tachometer_Calc{
	double tachometer;
};

Arbitrary_Data tachometer_calculation(Arbitrary_Data raw);
void tachometer_parse(uint32_t * buffer, Arbitrary_Data raw);

#endif // TACHOMETER_H
