#include "brakes.h"

using namespace BlackLib;

brake_control::brake_control(BlackLib::gpioName power, BlackLib::gpioName flip_flop)
			: power(power,output, FastMode) , flip_flop(flip_flop,output,FastMode)
{
	
}

// Turn on power, turn off flipflop
void brake_control::forward() {
	power.setValue( BlackLib::high );
	flip_flop.setValue( BlackLib::low );
}

// Turn on power, turn on flipflop
void brake_control::backward() {
	power.setValue( BlackLib::high );
	flip_flop.setValue( BlackLib::high );
}

// turn off power, turn off flipflop
void brake_control::stop() {	
	power.setValue( BlackLib::low );
	flip_flop.setValue( BlackLib::low );
}
		
