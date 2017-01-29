#include <SPI.h>
#include <Wire.h>

int lev_motor_pins[6] = {A0, A1, A2, A3, A4, A5};


int lev_voltages[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(0x25);
  Wire.onRequest(beagleTransmit);
  Wire.onReceive(beagleReceive);
}

uint8_t which = 0;
void beagleReceive(int input){
  which = Wire.read();
}

void beagleTransmit(){
   Wire.write(lev_voltages[which]);
   Wire.write(lev_voltages[which] >> 8);
 
}
void loop() {
  for(int i = 0; i < 6; i++){
    int val = analogRead(lev_motor_pins[i]);
    lev_voltages[i] = val;
  }


}
