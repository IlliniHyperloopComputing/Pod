#include "Temperature.h"

#define INITIAL_TEMPERATURE_OFFSET 0
#define TEMPERATURE_OFFSET 0

using namespace std;

void temperature_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Temperature_Raw * t_raw = (Temperature_Raw *)raw.data;
	t_raw->brake_temp = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET);
	t_raw->battery_temp = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + TEMPERATURE_OFFSET);
	t_raw->cpu_temp = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + 2*TEMPERATURE_OFFSET);
	t_raw->motor_temp = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + 3*TEMPERATURE_OFFSET);
	t_raw->wheel_temp = (uint8_t) *(buffer + INITIAL_TEMPERATURE_OFFSET + 4*TEMPERATURE_OFFSET);

}
