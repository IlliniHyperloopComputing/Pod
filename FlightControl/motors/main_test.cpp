#include <iostream>
#include "motors.h"

int main(){

	motor_control x(BlackLib::pwmName::P9_16, 1000, 700, 700, 2000);
	BlackLib::BlackPWM * y = x.tmp_testing_get_pwm_ptr();

	std::cout << "helllooo" << std::endl;
	std::cout << y->getNumericDutyValue() << std::endl;

}
