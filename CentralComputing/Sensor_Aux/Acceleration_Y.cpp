#include "Acceleration_Y.h"

#define ACCELERATION_Y_OFFSET 0

using namespace std;

void acceleration_y_parse(void * source, Arbitrary_Data raw) {
  Spi * spi = (Spi *) source;
	Acceleration_Y_Raw *ay_raw = (Acceleration_Y_Raw *)raw.data;
	ay_raw->one = spi->get_data(XMEGA1, Y_ACCELERATION_INDEX);
}
