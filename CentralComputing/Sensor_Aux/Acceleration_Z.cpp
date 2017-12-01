#include "Acceleration_Z.h"

#define ACCELERATION_Z_OFFSET 0

using namespace std;

void acceleration_z_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Acceleration_Z_Raw *az_raw = (Acceleration_Z_Raw *)raw.data;
	az_raw->one = *(((uint16_t *)buffer) + ACCELERATION_Z_OFFSET);
}
