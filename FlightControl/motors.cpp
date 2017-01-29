#include "motors.h"

motor_control::motor_control(enum BlackLib::pwmName pwm_pin, enum BlackLib::gpioName power_pinA, enum BlackLib::gpioName power_pinB, enum BlackLib::gpioName power_pinC, double low_us)
				: pwm_pin(pwm_pin), power_pinA(power_pinA), power_pinB(power_pinB), power_pinC(power_pinC),
			    pwm(pwm_pin),
                powerA(power_pinA,BlackLib::output,BlackLib::FastMode), 
                powerB(power_pinB,BlackLib::output,BlackLib::FastMode), 
                powerC(power_pinC,BlackLib::output,BlackLib::FastMode), 
			    low_us(low_us) 
{
    
    //std::cout<<"value of power pin"<<power.getValue()<<std::endl;    
	off();
    pwm.setDutyPercent(100.0);
	pwm.setPeriodTime(uint64_t(20), BlackLib::milisecond);
}

void motor_control::on(){
	powerA.setValue( BlackLib::high );
	powerB.setValue( BlackLib::high );
	powerC.setValue( BlackLib::high );

    //std::cout<<"value of power pin"<<power.getValue()<<std::endl;    
}

void motor_control::off(){
	powerA.setValue( BlackLib::low );
	powerB.setValue( BlackLib::low );
	powerC.setValue( BlackLib::low );
    //std::cout<<"value of power pin"<<power.getValue()<<std::endl;    
}

void motor_control::set_low(){
	set_microseconds( low_us );
}

void motor_control::set_microseconds(double microseconds){	
    double x = microseconds / 200.0;
    pwm.setDutyPercent(x);
}

