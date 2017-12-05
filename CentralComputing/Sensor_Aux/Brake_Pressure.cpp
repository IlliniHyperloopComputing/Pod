#include "Brake_Pressure.h"

#define BRAKE_PRESSURE_OFFSET 0

using namespace std;

void brake_pressure_parse(Spi * spi, Arbitrary_Data raw) {
	Brake_Pressure_Raw * brake_raw = (Brake_Pressure_Raw *)raw.data;
	brake_raw->brake_pressure = spi->get_data(XMEGA1, BRAKE_PRESSURE_INDEX);
}
