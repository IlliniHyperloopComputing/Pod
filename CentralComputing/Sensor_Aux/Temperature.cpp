#include "Temperature.h"

#define TEMPERATURE_EXT_OFFSET 0
#define TEMPERATURE_INT_OFFSET 0

using namespace std;

Arbitrary_Data temperature_calculation(Arbitrary_Data raw) {
	Temperature_Raw t_raw = *(Temperature_Raw *)raw.data;
	double temperature_ext = d_raw;
	double temperature_int = d_raw;
	//insert calculation code if ever need here
	Temperature_Calc * t_calc = (Temperature_Calc *) malloc(sizeof(Temperature_Calc));
	t_calc->temperature_ext=temperature_ext;
	t_calc->temperature_int=temperature_int;
	Arbitrary_Data calculated;
	calculated.size = sizeof(Temperature_Calc);
	calculated.data = (uint8_t *)d_calc;
	return calculated;
}

void temperature_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Temperature_Raw * t_raw = (Temperature_Raw *)raw.data;
	t_raw->temperature_ext = (double) *(buffer + TEMPERATURE_EXT_OFFSET);
	t_raw->temperature_int = (double) *(buffer + TEMPERATURE_INT_OFFSET);
}
