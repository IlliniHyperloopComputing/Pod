#include <SPI.h>
#include <Wire.h>
unsigned long last_time = 0;

int pins[4] = {7, 0, 0, 0};
int counts[4]; //how many times has it sensed a
int high[4]; //whether the pin is high or not
long risingTimes[4];
int i = 0;
void setup() {
  // put your setup code here, to run once
  for(int i = 0; i < 4; i++){
    counts[i] = 0;
    high[i] = 0;
  }
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
  Wire.write(counts[which]);
  Wire.write(counts[which]);
  
  
}

void loop(){
  int value = digitalRead(pins[i]);
  if(value == HIGH && !high[i]){
    //rising edge
    risingTimes[i] = micros();
    high[i] = 1;
  } else if(value == LOW && high[i]){
    //falling edge
    //could calculate deltas using risingTimes[i]
    counts[i]++;
    high[i] = 0;
    Serial.print("Falling edge, counts[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(counts[i]);
  }
  //i = (i + 1) % 4;
}


