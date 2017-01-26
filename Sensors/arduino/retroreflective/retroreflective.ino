#include <SPI.h>
#include <Wire.h>
unsigned long last_time = 0;

//int pins[4] = {12, 0, 0, 0};
int pin = 12;
int count = 0;
int high = 0;
long risingTime;
/*
int counts[4]; //how many times has it sensed a
int high[4]; //whether the pin is high or not

long risingTimes[4];
*/
void setup() {
  // put your setup code here, to run once
  
  Serial.begin(9600);
  Serial.println("Starting");
  Wire.begin(0x17);
  Wire.onRequest(beagleTransmit);
  Wire.onReceive(beagleReceive);
  
}

uint8_t which = 0;
void beagleReceive(int numbytes){
  
}
void beagleTransmit() {
  //Serial.println("which");
  Wire.write(count);
  
  
}

void loop(){
  int value = digitalRead(pin);
  if(value == LOW && !high){
    //rising edge
    risingTime = micros();
    high = 1;
  } else if(value == HIGH && high){
    //falling edge
    //could calculate deltas using risingTimes[i]
    count++;
    high = 0;
/*
    if(count % 50 == 0){
      Serial.print("Falling edge, counts[");
      Serial.print(i);
      Serial.print("] = ");
      Serial.println(counts[i]);
    }
*/
    
  }
  //i = (i + 1) % 4;
}



