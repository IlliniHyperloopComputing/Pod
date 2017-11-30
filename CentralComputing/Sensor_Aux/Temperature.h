#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "../Data.h"
#include "../Sensor.h"

struct Temperature_Raw {
  uint8_t brake_temp;
  uint8_t battery_temp;
  uint8_t cpu_temp;
  uint8_t motor_temp;
  uint8_t wheel_temp;
};

void temperature_parse(Spi * spi , Arbitrary_Data raw);

#endif
