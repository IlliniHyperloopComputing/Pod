#ifndef DISTANCE_H
#define DISTANCE_H
#include "../Data.h"
#include "../Sensor.h"
struct Distance_Raw {
  uint32_t tape_count;
  uint32_t encoder_count;
};

struct Distance_Calc {
  double distance;
};

Arbitrary_Data distance_calculation(Arbitrary_Data raw);
void distance_parse(Spi * spi, Arbitrary_Data raw);

#endif // DISTANCE_H
