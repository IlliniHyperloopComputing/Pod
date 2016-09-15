#include "motors.h"

motor_control::motor_control(enum BlackLib::pwmName pwm_pin, uint16_t initial_us, uint16_t arm_us, uint16_t low_us, uint16_t high_us)
				: pwm_pin(pwm_pin), initial_us(initial_us), 
					arm_us(arm_us), low_us(low_us), 
					high_us(high_us), pwm(pwm_pin)
{
	pwm.setPeriodTime(uint64_t(20), BlackLib::timeType::milisecond);
	pwm.setDutyPercent(create_percent(this->initial_us));
}


void motor_control::raise_by_percent(float percent){

}

void motor_control::lower_by_percent(float percent){

}

float motor_control::get_percent(){
	return percent;
}

uint16_t motor_control::get_microseconds(){
	return current_us;
}

void motor_control::arm(){


}

BlackLib::BlackPWM * motor_control::tmp_testing_get_pwm_ptr(){
	return &pwm;

}


void motor_control::set_low(){

}

void motor_control::set_microseconds(uint16_t microseconds){

}

float motor_control::create_percent(uint16_t microseconds_in){
	
	//implement
	//Duty Cycle = TimeOn / Period
	//Period == 20 milliseconds
    //TimeOn 

	return 50;
}


