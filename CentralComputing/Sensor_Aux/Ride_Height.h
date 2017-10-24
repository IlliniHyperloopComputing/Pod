#ifndef RIDE_HEIGHT_H
#define RIDE_HEIGHT_H
#include "../Data.h"

struct Ride_Height_Raw {
  uint16_t left;
  uint16_t middle;
  uint16_t right; 
};

void ride_height_parse(uint8_t * buffer, Arbitrary_Data raw);
#endif
