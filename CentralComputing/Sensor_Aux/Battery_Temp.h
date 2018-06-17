#ifndef BATTERY_TEMP_H
#define BATTERY_TEMP_H
#include "../Data.h"
#include "../Sensor.h"

//struct Battery_Temp_Raw {
//  uint8_t cell[8];
//};

//struct Battery_Temp_Calc {
//  uint8_t avg_temp;
//};

Arbitrary_Data battery_temp_calculation(Arbitrary_Data raw);
void battery_temp_parse(void * source, Arbitrary_Data raw); 
#endif
