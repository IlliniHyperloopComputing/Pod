#include "motors.h"

motor_control::motor_control(enum BlackLib::pwmName pwm_pin, enum BlackLib::gpioName power_pin, double low_us)
				: pwm_pin(pwm_pin), power_pin(power_pin),
			    pwm(pwm_pin),
                power(power_pin,BlackLib::output,BlackLib::FastMode), 
			    low_us(low_us) 
{
	off();
    pwm.setDutyPercent(100.0);
	pwm.setPeriodTime(uint64_t(20), BlackLib::milisecond);
}

void motor_control::on(){
	power.setValue( BlackLib::high );
}

void motor_control::off(){
	power.setValue( BlackLib::low );
}

void motor_control::set_low(){
	set_microseconds( low_us );
}

void motor_control::set_microseconds(double microseconds){	
    double x = microseconds / 200.0;
    pwm.setDutyPercent(x);
}

