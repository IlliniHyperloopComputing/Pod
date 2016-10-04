
#ifndef BRAKES_H
#define BRAKES_H

#include <stdint.h>
#include "../../BlackLib/v3_0/BlackGPIO/BlackGPIO.h"

// brake control. 
// when this object is initialized, it has the ability to control the brakes
// Controls both the switch and the flipflop for our brake controls

class brake_control{
	public:
		// Constructor Takes in two pins which control the power and the flipflop
		// Power controls whether or not the brakes will be operable
		// Flip flop controls the direction the brakes will move
		//	off is forward, on is backward
		brake_control(enum BlackLib::gpioName power, enum BlackLib::gpioName flip_flop);

		// Turn on power, turn off flipflop
		void forward();

		// Turn on power, turn on flipflop
		void backward();

		// turn off power, turn off flipflop
		void stop();
		
	private:

		BlackLib::BlackGPIO power;
		BlackLib::BlackGPIO flip_flop;
		
		enum BlackLib::gpioName power_pin;
		enum BlackLib::gpioName flip_flop_pin;

};

#endif
