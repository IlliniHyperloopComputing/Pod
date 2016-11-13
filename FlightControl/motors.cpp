#include "motors.h"

motor_control::motor_control(enum BlackLib::pwmName pwm_pin, enum BlackLib::gpioName power, double initial_us, double arm_us, double low_us, double high_us)
				: pwm_pin(pwm_pin), power(power,BlackLib::output,BlackLib::FastMode), initial_us(initial_us), 
					arm_us(arm_us), low_us(low_us), 
					high_us(high_us), pwm(pwm_pin)
{
	off();
    pwm.setDutyPercent(100.0);
	pwm.setPeriodTime(uint64_t(20), BlackLib::milisecond);
    std::cout<< pwm.getDutyValue() << std::endl;
	//pwm.setDutyPercent(create_percent(this->initial_us));
}


void motor_control::arm(){
	is_armed = true;	
	set_microseconds( arm_us );
}

BlackLib::BlackPWM * motor_control::tmp_testing_get_pwm_ptr(){
	return &pwm;

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
/*
    pwm.setDutyPercent(100.0);
	pwm.setPeriodTime(uint64_t(20), BlackLib::milisecond);
*/

    double x = microseconds / 200.0;//FUCK THIS STUPID FUCKING LINE
    pwm.setDutyPercent(x);

}

