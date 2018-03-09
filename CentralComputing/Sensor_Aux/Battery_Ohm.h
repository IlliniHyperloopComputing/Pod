#ifndef BATTERY_OHM_H
#define BATTERY_OHM_H
#include "../Data.h"
#include "../Sensor.h"


struct Battery_Ohm_Calc {
  uint16_t pack_resistance;
};

Arbitrary_Data battery_ohm_calculation(Arbitrary_Data raw);
void battery_ohm_parse(void * source, Arbitrary_Data raw); 
#endif
