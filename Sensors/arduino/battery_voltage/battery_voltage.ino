#include <SPI.h>
#include <Wire.h>

int lev_motor_pins[3] = {A0, A1, A2};
int sta_motor_pins[2] = {A3, A4};

int lev_voltages[3] = {0, 0, 0};
int sta_voltages[2] = {0, 0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(0x21);
  Wire.onRequest(beagleTransmit);
  Wire.onReceive(beagleReceive);
}

uint8_t which = 0;
void beagleReceive(int input){
  which = Wire.read();
}

void beagleTransmit(){
  if(which < 3){
    Wire.write(lev_voltages[which]);
    Wire.write(lev_voltages[which] >> 8);
  } else if(which < 5){
    Wire.write(sta_voltages[which % 3]);
    Wire.write(sta_voltages[which % 3] >> 8);
  }
}
void loop() {
  for(int i = 0; i < 3; i++){
    int val = analogRead(lev_motor_pins[i]);
    lev_voltages[i] = val;
  }

  for(int i = 0; i < 2; i++){
    int val = analogRead(sta_motor_pins[i]);
    sta_voltages[i] = val;  
  }

}
