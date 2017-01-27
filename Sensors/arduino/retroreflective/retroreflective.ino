#include <SPI.h>
#include <Wire.h>
unsigned long last_time = 0;

//int pins[4] = {12, 0, 0, 0};
int pin = 12;
int count = 0;
int high = 0;
long risingTime = 0;
long delta = 0;

void setup() {
  // put your setup code here, to run once
  
  Serial.begin(9600);
  Serial.println("Starting");
  Wire.begin(0x17);
  Wire.onRequest(beagleTransmit);
  Wire.onReceive(beagleReceive);
  
}

uint8_t which = 0;
void beagleReceive(int input){
  uint8_t val = Wire.read();
  if(val == 0xff){
    //reset the count
    count = 0;
  } else if (val == 0 || val == 1) {
    which = val;
  }
}
void beagleTransmit() {
  //Serial.println("which");
  if(which == 0){
    Wire.write(count);
  } else if (which == 1) {
    Wire.write(delta);
  }
  
  
}

void loop(){
  int value = digitalRead(pin);
  if(value == LOW && !high){
    //rising edge
    delta = millis() - risingTime;
    risingTime = millis();
    high = 1;
  } else if(value == HIGH && high){
    //falling edge
    //could calculate deltas using risingTimes[i]
    count++;
    high = 0;

    //if(count % 50 == 0){
      Serial.print("Falling edge, count = ");
      Serial.println(count);
    //}

    
  }
}



