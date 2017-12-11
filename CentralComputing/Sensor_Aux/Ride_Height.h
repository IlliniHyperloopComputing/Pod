#ifndef RIDE_HEIGHT_H
#define RIDE_HEIGHT_H
#include "../Data.h"
#include "../Sensor.h"

struct Ride_Height_Raw {
  uint16_t left;
  uint16_t middle;
  uint16_t right; 
};

void ride_height_parse(Spi * spi, Arbitrary_Data raw);
#endif
