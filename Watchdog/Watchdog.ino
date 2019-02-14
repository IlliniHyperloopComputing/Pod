int inPin = 7; //whatever the input pin is
int outPin = 13; //output to tell other hardware if software failed
int heartBeat = 0;   //output value, start LOW
int lastBeat = 0; //to check for changing vals
int changeCounter = 0; //just looking for changes
int totalCounter = 0; //to check if we are changing fast enough
int mainLoop = 0; //turn on once we know program is running

//wait for X amount of signal changes (heart beats) within a short 
//time period (to make sure power cycles or other transients doesn't 
//accidentally turn it on)
//no idea what these should be
int waitPeriod = 15;
int goal = 10; 
int state = 0;
  
  
void setup() {
  pinMode(outPin, OUTPUT);
  pinMode(inPin, INPUT);
  //START LOW
  digitalWrite(outPin, 0);
}

void loop() {
  switch(state){
    
    //NOT STARTED YET
    case 0:
      //check for change in hearbeat
      heartBeat = digitalRead(inPin);
      if (heartBeat != lastBeat){ changeCounter++; }
      totalCounter++;
      lastBeat = heartBeat;
    
      //if we get 10 changes in a span of 15 sec continue to healthy state
      if (changeCounter > goal){
        state = 1;
      }
      
      //reset counters to continue waiting
      if (totalCounter > waitPeriod){
        totalCounter = 0;
        changeCounter = 0;
      }
      break;
      
    //HEALTHY STATE
    case 1:
      //might want to add some tolerance here
      digitalWrite(outPin, 1);
      heartBeat = digitalRead(inPin);
      if (heartBeat == lastBeat){
        state = 2;
      }
    
      lastBeat = heartBeat;
      break;
      
    //DEAD STATE
    case 2:
        digitalWrite(outPin, 0);
        while (1){ 
          //DO NOTHING 
        }
  }
}

