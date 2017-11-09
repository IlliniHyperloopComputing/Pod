#ifndef CURRENT_H
#define CURRENT_H
#include "../Data.h"

struct Current_Raw {
	uint16_t current;
};

void current_parse(uint8_t * buffer, Arbitrary_Data raw);
#endif
