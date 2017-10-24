#include "Ride_Height.h"

#define RIDE_HEIGHT_OFFSET 0

using namespace std;

Arbitrary_Data ride_height_calculation(Arbitrary_Data raw) {
	Ride_Height_Raw rh_raw = *(Ride_Height_Raw *) raw.data;
	Ride_Height_Calc * rh_calc = (Ride_Height_Calc *) malloc(sizeof(Ride_Height_Calc));
	rh_calc->height = rh_raw.height;
	Arbitrary_Data calculated;
	calculated.size = sizeof(Ride_Height_Calc);
	calculated.data = (uint8_t *)rh_calc;
	return calculated;
}

void ride_height_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Ride_Height_Raw * rh_raw = (Ride_Height_Raw *) raw.data;
	rh_raw->height = (double) *(buffer + RIDE_HEIGHT_OFFSET);
}
