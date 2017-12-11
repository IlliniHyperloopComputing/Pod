#include "Temperature.h"

#define TEMPERATURE_OFFSET 0

using namespace std;

void temperature_parse(Spi * spi, Arbitrary_Data raw) {
  Temperature_Raw * t_raw = (Temperature_Raw *)raw.data;
  t_raw->brake_temp = spi->get_data(XMEGA2, THERMOCOUPLE_INDEX_1);
  t_raw->battery_temp = spi->get_data(XMEGA2, THERMOCOUPLE_INDEX_2);
  t_raw->cpu_temp = spi->get_data(XMEGA2, THERMOCOUPLE_INDEX_3);
  t_raw->motor_temp = spi->get_data(XMEGA2, THERMOCOUPLE_INDEX_4);
  t_raw->wheel_temp = spi->get_data(XMEGA2, THERMOCOUPLE_INDEX_5);

}
