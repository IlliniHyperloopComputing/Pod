#include "Brake_Pressure.h"

#define BRAKE_PRESSURE_OFFSET 0

using namespace std;

void brake_pressure_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Brake_Pressure_Raw * brake_raw = (Brake_Pressure_Raw *)raw.data;
	brake_raw->brake_pressure = *(((uint16_t *)buffer) + BRAKE_PRESSURE_OFFSET);
}
