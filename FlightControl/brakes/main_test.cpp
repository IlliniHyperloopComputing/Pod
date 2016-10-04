#include <iostream>
#include "brakes.h"
#include "unistd.h"
#include "../../BlackLib/v3_0/BlackGPIO/BlackGPIO.h"

using namespace std;
int main(){
	
	
	brake_control x(BlackLib::gpioName::GPIO_67, BlackLib::gpioName::GPIO_68);	
		
	cout << "Setting brakes to stop" << endl;
	x.stop();
	usleep(10000000);	
	
	cout << "Starting loop" << endl;
	while (true) {
		cout << "Stopping brakes" << endl;
		x.stop();
		usleep(3000000);	
		
		cout << "Setting brakes forward" << endl;
		x.forward();
		usleep(3000000);
		
		cout << "Stopping brakes" << endl;
		x.stop();
		usleep(3000000);	
		
		cout << "Setting brakes backward" << endl;
		x.backward();
		usleep(3000000);
		
		cout << "Setting brakes forward" << endl;
		x.forward();
		usleep(3000000);		
		
		cout << "Setting brakes backward" << endl;
		x.backward();
		usleep(3000000);
	}
}
