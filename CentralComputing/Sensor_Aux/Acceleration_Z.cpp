#include "Acceleration_Z.h"

#define ACCELERATION_Z_OFFSET 0

using namespace std;

void acceleration_z_parse(Spi * spi, Arbitrary_Data raw) {
	Acceleration_Z_Raw *az_raw = (Acceleration_Z_Raw *)raw.data;
	az_raw->one = spi->get_data(XMEGA2, Z_ACCELERATION_INDEX);
}
