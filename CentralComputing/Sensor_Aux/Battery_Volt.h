#ifndef BATTERY_VOLT_H
#define BATTERY_VOLT_H
#include "../Data.h"
#include "../Sensor.h"

struct Battery_Volt_Raw {
  uint16_t pack_voltage;
};

struct Battery_Volt_Calc {
  uint8_t cell[8];
};

Arbitrary_Data battery_volt_calculation(Arbitrary_Data raw);
void battery_volt_parse(void * source, Arbitrary_Data raw); 
#endif
