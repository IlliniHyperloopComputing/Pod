#include "Temperature.h"

#define TEMPERATURE_OFFSET 0

using namespace std;

void temperature_parse(uint8_t * buffer, Arbitrary_Data raw) {
  Temperature_Raw * t_raw = (Temperature_Raw *)raw.data;
  t_raw->brake_temp = *(buffer + TEMPERATURE_OFFSET);
  t_raw->battery_temp = *(buffer + TEMPERATURE_OFFSET + 1);
  t_raw->cpu_temp = *(buffer + TEMPERATURE_OFFSET + 2);
  t_raw->motor_temp = *(buffer + TEMPERATURE_OFFSET + 3);
  t_raw->wheel_temp = *(buffer + TEMPERATURE_OFFSET + 4);
}
