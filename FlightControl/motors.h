#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>
#include "../BlackLib/v3_0/BlackPWM/BlackPWM.h"
#include "../BlackLib/v3_0/BlackGPIO/BlackGPIO.h"

//motor control. 
//when this object is initialized, it has the ability to control any of our motors
//50HZ PWM signal
//PWM control through BlackLib

class motor_control{
	public:
		//Constructor
		//all time is measured in micro-seconds --- Greek leter mu, abbreviated u
		//initial duty cycle -- different between levitiation and stability
		//arm duty cycle -- switch to this duty cycle from initial to arm the motor
		//low duty cycle -- the lowest the motor can go 
		//high duty cycle -- highest the motor can go 
		motor_control(enum BlackLib::pwmName pwm_pin,enum BlackLib::gpioName power_pin, double low_us);

		// turns the motor relay on, gpio pin high
		void on();
		// turns the motor relay off, gpio pin low
		void off();

		//will set the motor on low power
		void set_low();

		void set_microseconds( double microseconds);
		
	private:
		//Data
		enum BlackLib::pwmName pwm_pin;	
		enum BlackLib::gpioName power_pin;
		
		BlackLib::BlackPWM  pwm;
		BlackLib::BlackGPIO power;

		double low_us;
};

#endif
