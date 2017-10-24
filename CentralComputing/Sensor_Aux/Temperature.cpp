#include "Temperature.h"

#define TEMPERATURE_OFFSET 0

using namespace std;

void temperature_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Temperature_Raw * t_raw = (Temperature_Raw *)raw.data;
	t_raw->brake_temp = (uint8_t) *(buffer + TEMPERATURE_OFFSET);
	t_raw->battery_temp = (uint8_t) *(buffer + TEMPERATURE_OFFSET + sizeof(uint8_t));
	t_raw->cpu_temp = (uint8_t) *(buffer + TEMPERATURE_OFFSET + 2*sizeof(uint8_t));
	t_raw->motor_temp = (uint8_t) *(buffer + TEMPERATURE_OFFSET + 3*sizeof(uint8_t));
	t_raw->wheel_temp = (uint8_t) *(buffer + TEMPERATURE_OFFSET + 4*sizeof(uint8_t));
}
