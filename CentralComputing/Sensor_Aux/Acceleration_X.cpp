#include "Acceleration_X.h"



using namespace std;

const double VOLTS_PER_G = 0.330;

Arbitrary_Data acceleration_x_calculation(Arbitrary_Data raw) {
	Acceleration_X_Raw ax_raw = *(Acceleration_X_Raw *)raw.data;
	double accel = 0;
	if(ax_raw.one > ax_raw.two){
		if(ax_raw.two > ax_raw.three){
			accel = ax_raw.two;
		}
		else if(ax_raw.one > ax_raw.three){
			accel = ax_raw.three;
		}
		else{
			accel = ax_raw.one;
		}
	}
	else{
		if(ax_raw.one > ax_raw.three){
			accel = ax_raw.one;
		}
		else if(ax_raw.two > ax_raw.three){
			accel = ax_raw.three;
		}
		else{
			accel = ax_raw.two;
		}
	}
	if(ax_raw.current_samples > 0) {
		ax_raw.calibrated_baseline += accel;
		ax_raw.current_samples--;
		if(ax_raw.current_samples == 0){
			ax_raw.calibrated_baseline /= ax_raw.total_samples;
			ax_raw.calibrated = true;
		}
	}

	if(ax_raw.calibrated){
		accel = ((ax_raw.calibrated_baseline-accel)/32768.0 * 4.096)/VOLTS_PER_G;
	}
	else {
		accel = 0;
	}
	Acceleration_X_Calc * ax_calc = (Acceleration_X_Calc *)malloc(sizeof(Acceleration_X_Calc));
	ax_calc->x = accel;
	Arbitrary_Data calculated;
	calculated.size = sizeof(Acceleration_X_Calc);
	calculated.data = (uint8_t *)ax_calc;
	return calculated;
}

void acceleration_x_parse(Spi* spi, Arbitrary_Data raw) {
	Acceleration_X_Raw * ax_raw = (Acceleration_X_Raw *)raw.data;
  ax_raw->one = spi->get_data(XMEGA1, X_ACCELERATION_INDEX_1);
  ax_raw->two = spi->get_data(XMEGA1, X_ACCELERATION_INDEX_2);
  ax_raw->three = spi->get_data(XMEGA1, X_ACCELERATION_INDEX_3);
}
