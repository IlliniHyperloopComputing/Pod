#ifndef BATTERY_FLAGS_H
#define BATTERY_FLAGS_H
#include "../Data.h"
#include "../Sensor.h"


struct Battery_Flags_Calc {
	uint8_t state_of_system;
  uint8_t flags;
  uint8_t fault_code;
  uint8_t level_fault;
  uint8_t warnings;
};

Arbitrary_Data battery_flags_calculation(Arbitrary_Data raw);
void battery_flags_parse(void * source, Arbitrary_Data raw); 
#endif
