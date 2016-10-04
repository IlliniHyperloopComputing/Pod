#include <iostream>
#include "motors.h"
#include "unistd.h"
#include "../../BlackLib/v3_0/BlackPWM/BlackPWM.h"

using namespace std;
int main(){
	
	
	motor_control x(BlackLib::pwmName::P9_21, 1000.0, 700.0, 700.0, 200000.0);
	BlackLib::BlackPWM * y = x.tmp_testing_get_pwm_ptr();			
	
	x.arm();	
	while (true) {
		
		//x.raise_by_percent( 1 );
		cout << "NEW CYCLE-T" << endl;
		//y->setDutyPercent(50);
		
		//x.set_microseconds( 1500 );
	

		for(float i  = 0; i <= 120; i+= 1){
			cout << x.get_percent() << endl;
			x.raise_by_percent(.01);	
			usleep(100000);
		}

	
		
		for(float i  = 0; i <= 120; i+= 1){
			cout << x.get_percent() << endl;
			x.lower_by_percent(.01);		
			usleep(100000);
		}
	}
}
