#ifndef BATTERY_INFO_H
#define BATTERY_INFO_H
#include "../Data.h"
#include "../Sensor.h"

struct Battery_Info_Calc {
  uint16_t dod;
  uint16_t capacity;
  uint16_t soc;
  uint16_t soh;
};

Arbitrary_Data battery_info_calculation(Arbitrary_Data raw);
void battery_info_parse(void * source, Arbitrary_Data raw); 
#endif
