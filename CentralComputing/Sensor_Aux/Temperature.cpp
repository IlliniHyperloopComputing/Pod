#include "Temperature.h"

#define INITIAL_TEMPERATURE_OFFSET 0
#define TEMPERATURE_OFFSET 0

using namespace std;

void temperature_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Temperature_Raw * t_raw = (Temperature_Raw *)raw.data;
	t_raw->temperature_brakes = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET);
	t_raw->temperature_batteries = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + TEMPERATURE_OFFSET);
	t_raw->temperature_cpu = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + 2*TEMPERATURE_OFFSET);
	t_raw->temperature_motor = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + 3*TEMPERATURE_OFFSET);
	t_raw->temperature_wheels = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + 4*TEMPERATURE_OFFSET);

}
