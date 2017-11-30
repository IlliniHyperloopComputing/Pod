#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <SPI.h>

#define loops 500
int current = LOW;
unsigned long last;
unsigned long rpm;
uint16_t analog_voltage = 8;
uint16_t analog_amps = 8;
int rpm_pin = 12;
int voltage_pin = 0;
int amps_pin = 1;
int i = 0;
int debounce = 0;

void setup() {
  last = micros();
  pinMode(rpm_pin, INPUT);
  Serial.begin(115200);  
}


void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(rpm_pin);
  if(val == HIGH && current == LOW) {
    //rising edge
    current = HIGH;
    unsigned long now = micros();
    unsigned long delta = now - last;
    last = now;
    rpm = 60000000 / delta;
  } else if(val == LOW && current == HIGH){
    //falling edge
    current = LOW;
  }

  //Sample power at same rate that data is sent
  //Just sample at a different time
  if(i == loops/4){
    analog_voltage = analogRead(voltage_pin);
    
  }
  if(i == loops/2){
    analog_amps = analogRead(amps_pin);
  }

  i++;
  if(i == loops){
    Serial.print(rpm);
    Serial.print(" ");
    Serial.print(analog_voltage);
    Serial.print(" ");
    Serial.println(analog_amps);
    i = 0;
  }
}
