#include <SPI.h>
#include <Wire.h>

int highs[6];
int pins[6] = {13, 12, 8, 7, 4, 2};
unsigned long rising_times[6];
int rpms[6];
int loops;
void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < 6; i++){
    rising_times[i] = 0;
    highs[i] = 0;
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
  for(int i = 0; i < 6; i++){
    int value = digitalRead(pins[i]);
    if(value == HIGH && highs[i] == 0){
      highs[i] = 1;
      unsigned long now = micros();
      unsigned long delta = now - rising_times[i];
      noInterrupts();
      rpms[i] = 60000000 / delta;
      interrupts();
      
    }
    if(value == LOW && highs[i] == 1){
      highs[i] = 0;
    }
      
  }
}
