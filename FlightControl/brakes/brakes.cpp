include "motors.h"

using namespace BlackLib;

motor_control(enum BlackLib::gpioName power, enum BlackLib::gpioName flip_flop)
			: power(power) , flip_flop(flip_flop)
{
	
}

// Turn on power, turn off flipflop
void forward() {
	power.setValue( BlackLib::high );
	flip_flop.setValue( BlackLib::low );
}

// Turn on power, turn on flipflop
void backward() {
	power.setValue( BlackLib::high );
	flip_flop.setValue( BlackLib::high );
}

// turn off power, turn off flipflop
void stop() {	
	power.setValue( BlackLib::low );
	flip_flop.setValue( BlackLib::low );
}
		
