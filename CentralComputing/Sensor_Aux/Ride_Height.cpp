#include "Ride_Height.h"

#define RIDE_HEIGHT_OFFSET 0

using namespace std;

void ride_height_parse(void * source , Arbitrary_Data raw) {
  Spi * spi = (Spi *) source;
  Ride_Height_Raw * rh_raw = (Ride_Height_Raw *) raw.data;
  rh_raw->left = spi->get_data(XMEGA2, RIDE_HEIGHT_INDEX_1);
  rh_raw->middle = spi->get_data(XMEGA2, RIDE_HEIGHT_INDEX_2);
  rh_raw->right = spi->get_data(XMEGA2, RIDE_HEIGHT_INDEX_3);
}
