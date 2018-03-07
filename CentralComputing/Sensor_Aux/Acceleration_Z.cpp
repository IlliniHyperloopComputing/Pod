#include "Acceleration_Z.h"

#define ACCELERATION_Z_OFFSET 0

using namespace std;

void acceleration_z_parse(void * source, Arbitrary_Data raw) {
  Spi * spi = (Spi *) source;
	Acceleration_Z_Raw *az_raw = (Acceleration_Z_Raw *)raw.data;
	az_raw->one = spi->get_data(XMEGA1, Z_ACCELERATION_INDEX);
}
