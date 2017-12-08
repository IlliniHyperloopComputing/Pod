#include <Adafruit_ADS1015.h>
#include <Wire.h>
#include <SPI.h>

#define loops 100
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
int one_in_10 = 0;

Adafruit_ADS1115 ads;

void setup() {
  last = micros();
  pinMode(rpm_pin, INPUT);
  Serial.begin(115200);  
  ads.begin();
}


void loop() {


  //Sample power at same rate that data is sent  
  //Just sample at a different time
  if(i == loops/4){
    //analog_voltage = analogRead(voltage_pin);
    analog_voltage = ads.readADC_SingleEnded(0);
 
    
  }
  
  if(i == loops/2){
    //analog_amps = analogRead(amps_pin);
    analog_amps = ads.readADC_SingleEnded(1);
  }

  i++;
  if(i == loops){
    Serial.print(analog_voltage);
    Serial.print(" ");
    Serial.println(analog_amps);
    i = 0;
  }
}
