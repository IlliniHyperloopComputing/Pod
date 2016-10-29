#include "motors.h"

motor_control::motor_control(enum BlackLib::pwmName pwm_pin, double initial_us, double arm_us, double low_us, double high_us)
				: pwm_pin(pwm_pin), initial_us(initial_us), 
					arm_us(arm_us), low_us(low_us), 
					high_us(high_us), pwm(pwm_pin)
{
	pwm.setPeriodTime(uint64_t(20), BlackLib::milisecond);
	pwm.setDutyPercent(create_percent(this->initial_us));
}


void motor_control::raise_by_percent(double percent){
	double change_us = ( percent * ( high_us - low_us ) );
	double new_us = current_us + change_us;
	
	if ( new_us > high_us ) {
		new_us = high_us;
	}


	if ( new_us < low_us ) {
		new_us = low_us;
	}
	
	set_microseconds( new_us );
}

void motor_control::lower_by_percent(double percent){
	double change_us = ( percent *( high_us - low_us ) );
	double new_us = current_us - change_us;
	
	if ( new_us > high_us ) {
		new_us = high_us;
	}
	
	if ( new_us < low_us ) {
		new_us = low_us;
	}
	
	set_microseconds( new_us );
}

// Updates the percentage, doesn't control pod
// Sets percentage of the range of possible duty cycle values
void motor_control::update_percent( double new_us ) {
	percent = ( new_us - low_us ) / ( high_us - low_us );
}

double motor_control::get_percent(){
	return percent;
}

double motor_control::get_microseconds(){
	return current_us;
}

void motor_control::arm(){
	is_armed = true;	
	set_microseconds( arm_us );
	update_percent( arm_us );
}

void motor_control::disarm(){
	is_armed = false;
	set_microseconds( 0 );
}

BlackLib::BlackPWM * motor_control::tmp_testing_get_pwm_ptr(){
	return &pwm;

}


void motor_control::set_low(){
	
	set_microseconds( low_us );

}

void motor_control::set_microseconds(double microseconds){	
    std::cout << "Setting motor microseconds to : " << microseconds << std::endl;
	update_percent( microseconds );
	current_us = microseconds;
	pwm.setDutyPercent(create_percent(microseconds));

}

double motor_control::create_percent(double microseconds_in){
	
	double percent_us = ( microseconds_in - low_us ) / ( high_us - low_us );

	return percent_us;
}
