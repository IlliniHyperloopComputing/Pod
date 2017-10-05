#ifndef DISTANCE_H
#define DISTANCE_H
#include "../Data.h"
struct Distance_Raw {
  int tape_count;
  int encoder_count;
};

struct Distance_Calc {
  double distance;
};

Arbitrary_Data distance_calculation(Arbitrary_Data raw);
void distance_parse(uint8_t * buffer, Arbitrary_Data raw);

#endif // DISTANCE_H
