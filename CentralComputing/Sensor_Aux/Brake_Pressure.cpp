#include "Brake_Pressure.h"

#define BRAKE_PRESSURE_OFFSET 0

using namespace std;

void brake_pressure_parse(void * source, Arbitrary_Data raw) {
  Spi * spi = (Spi *) source;
	Brake_Pressure_Raw * brake_raw = (Brake_Pressure_Raw *)raw.data;
	brake_raw->brake_pressure = spi->get_data(XMEGA1, BRAKE_PRESSURE_INDEX);
}
