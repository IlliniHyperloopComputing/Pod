#include <iostream>
#include "motors.h"

using namespace std;
int main(){

	motor_control x(BlackLib::pwmName::P9_16, 1000, 700, 700, 2000);
	BlackLib::BlackPWM * y = x.tmp_testing_get_pwm_ptr();		
	
	for(float i  = 0; i <= 100; i+= 0.5){
		cout << "i = " << i << endl;
		x.raise_by_percent(i);	
	}

}
