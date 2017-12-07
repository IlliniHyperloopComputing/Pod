#ifndef TACHOMETER_h
#define TACHOMETER_h
#include "../Data.h"
#include "../Sensor.h"

//holds raw rpms as measured
struct Tachometer_Raw{
	
	uint32_t left;
  uint32_t middle;
  uint32_t right; 
  uint32_t lfront; 
  uint32_t rfront; 
  uint32_t back; 
};

struct Tachometer_Calc{
};

Arbitrary_Data tachometer_calculation(Arbitrary_Data raw);
void tachometer_parse(Spi * spi, Arbitrary_Data raw);

#endif // TACHOMETER_H
