#include "Acceleration_Y.h"

#define ACCELERATION_Y_OFFSET 0

using namespace std;

void acceleration_y_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Acceleration_Y_Raw *ay_raw = (Acceleration_Y_Raw *)raw.data;
	ay_raw->one = *(((uint16_t *)buffer)+ACCELERATION_Y_OFFSET);
}
