#ifndef BATTERY_AMP_H
#define BATTERY_AMP_H
#include "../Data.h"
#include "../Sensor.h"


struct Battery_Amp_Calc {
  uint16_t pack_current;
  uint16_t charge_limit;
  uint16_t discharge_limit;
};

Arbitrary_Data battery_amp_calculation(Arbitrary_Data raw);
void battery_amp_parse(void * source, Arbitrary_Data raw); 
#endif
