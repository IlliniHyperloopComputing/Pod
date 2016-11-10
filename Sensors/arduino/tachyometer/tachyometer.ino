#include <SPI.h>
#include <Wire.h>


unsigned long times[4];
int pins[4] = {7, 0, 0, 0};
int rpms[4];
int loops;
void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 4; i++){
    times[i] = 0;
    rpms[i] = 0xbeef;
  }
  Serial.begin(9600);
  Serial.println("Starting");
  Wire.begin(0x16);
  Wire.onRequest(beagleTransmit);
  Wire.onReceive(beagleReceive);
  
}

uint8_t which = 0;
void beagleReceive(int numbytes){
  which = Wire.read();
}
void beagleTransmit() {
  //Serial.println("which");
  Wire.write(rpms[which]);
  Wire.write(rpms[which] >> 8);
  
}
void loop() {
  // put your main code here, to run repeatedly;
  for(int i = 0; i < 1; i++){
    
    if(digitalRead(pins[i]) == LOW){
      loops++;
      if(loops == 1000){
        Serial.print(" rpm = ");
        Serial.println(rpms[i]);
        loops = 0;
      }
      unsigned long now = micros();
      unsigned long delta = now - times[i];
      if(delta > 12000){
        times[i] = now;
        noInterrupts();
        rpms[i] = 60000000 / delta;
        interrupts();
        /*Serial.print("times[i] ");
        Serial.print(times[i]);
        //less than 5000rpm, gives signifcant time so no double hits
        times[i] = now;
        rpms[i] = 60000000 / delta;
        
        Serial.print(" now ");
        Serial.print(now);
        Serial.print(" delta = ");
        Serial.print(delta);
        Serial.print(" rpm = ");
        Serial.println(rpms[i]);*/
      }
    }
  }
}
