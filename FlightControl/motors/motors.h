#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>
#include <iostream>

namespace flight_control{

//motor control. 
//when this object is initialized, it has the ability to control any of our motors

class motor_control{
	public:
		//Constructor
		//initial duty cycle -- different between levitiation and stability
		//arm duty cycle -- switch to this duty cycle from initial to arm the motor
		//low duty cycle -- the lowest the motor can go 
		//high duty cycle -- highest the motor can go 
		motor_control(uint16_t initial, uint16_t arm, uint16_t low, uint16_t high);

		//Raise PWM pulse by x%.  100% == high, 0% = low
		void raise_by_percent(uint8_t percent); 
		void lower_by_percent(uint8_t percent);

		//Get percentage
		uint8_t get_percent();
		//Get microseconds
		uint16_t get_microseconds();
		void arm();
		void disarm();
		
	private:
		void set_microseconds(uint16_t microseconds);

		//Data
		uint16_t low_ms;
		uint16_t high_ms;
		uint16_t curent_ms;
		uint8_t percent;
	
	

}


}



#endif
