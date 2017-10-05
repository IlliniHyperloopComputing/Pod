#ifndef NULL_H
#define NULL_H
#include "../Data.h"

//NO CALCULATION
// This function is used by any data that does not need to be calculated
// On the pod. Instead, the conversion can happen during LabView
// Consider this a NULL case, kind of
Arbitrary_Data no_calculation(Arbitrary_Data raw);

#endif //NULL_H
