#ifndef SENSOR_AUX_H
#define SENSOR_AUX_H
#include "../Data.h"


//NO CALCULATION
// This function is used by any data that does not need to be calculated
// On the pod. Instead, the conversion can happen during LabView
// Consider this a NULL case, kind of
Arbitrary_Data no_calculation(Arbitrary_Data raw);

//DISTANCE
struct Distance_Raw {
  int tape_count;
  int encoder_count;
};

struct Distance_Calc {
  double distance;
}

Arbitrary_Data distance_calculation(Arbitrary_Data raw);
void distance_parse(uint8_t * buffer, Arbitrary_Data raw);

#endif //SENSOR_AUX_H
