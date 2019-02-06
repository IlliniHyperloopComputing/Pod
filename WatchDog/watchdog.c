nt inPin = 7; //whatever the input pin is
int outPin = 13; //output to tell other hardware if software failed
int state = 0;   //output value, start LOW
int lastState = 0; //to check for changing vals
int changeCounter = 0; //just looking for changes
int totalCounter = 0; //to check if we are changing fast enough
int mainLoop = 0; //turn on once we know program is running

//wait for X amount of signal changes (heart beats) within a short 
//time period (to make sure power cycles or other transients doesn't 
//accidentally turn it on)
//no idea what these should be
int waitPeriod = 15;
int goal = 10; 


void setup(){
	pinMode(outPin, OUTPUT);
	pinMode(inPin, INPUT);
}

void loop(){

	//might want to add some tolerance here
if (mainLoop){
	//HEALTHY
	digitalWrite(outPin, 1);
	state = digitalRead(inPin);
	if (state == lastState){
		//it broke
		while (1){
		digitalWrite(outPin, 0);
		}
	}

	lastState = state;
}

else{
	//reset period
	if (totalCounter > waitPeriod){
		totalCounter = 0;
		changeCounter = 0;
	}

	//check for change state
	state = digitalRead(inPin);
	if (state != lastState){
		changeCounter++;
	}
	totalCounter++;
	lastState = state;

	//START LOW
	digitalWrite(outPin, 0);

	//if we get 10 changes in a span of 15 sec continue to main
	if (changeCounter > goal){
		mainLoop = 1;
	}
}
}
