#include "Ride_Height.h"

#define RIDE_HEIGHT_OFFSET 0

using namespace std;

void ride_height_parse(uint8_t * buffer, Arbitrary_Data raw) {
  Ride_Height_Raw * rh_raw = (Ride_Height_Raw *) raw.data;
  rh_raw->left = *(((uint16_t *)buffer) + RIDE_HEIGHT_OFFSET);
  rh_raw->middle = *(((uint16_t *)buffer) + RIDE_HEIGHT_OFFSET + 1);
  rh_raw->right = *(((uint16_t *)buffer) + RIDE_HEIGHT_OFFSET + 2);
}
