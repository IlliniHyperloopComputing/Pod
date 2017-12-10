#include <Adafruit_ADS1015.h>
#include <Wire.h>
#include <SPI.h>

#define loops 100
int current = LOW;
unsigned long last;
unsigned long rpm;

uint16_t analog_voltage1 = 8;
uint16_t analog_amps1 = 8;

uint16_t analog_voltage2 = 8;
uint16_t analog_amps2 = 8;

int i = 0;

Adafruit_ADS1115 ads;

void setup() {
  last = micros();
  Serial.begin(115200);  
  ads.begin();
}


void loop() {


  //Sample power at same rate that data is sent  
  //Just sample at a different time
  if(i == loops/4){
    //analog_voltage = analogRead(voltage_pin);
    //analog_voltage1 = ads.readADC_SingleEnded(0);
 
    
  }
  
  if(i == loops/2){
    //analog_amps = analogRead(amps_pin);
    analog_amps1 = ads.readADC_SingleEnded(1);
  }
  
  
  if(i == loops/4 + 4){
    //analog_voltage = analogRead(voltage_pin);
    //analog_voltage2 = ads.readADC_SingleEnded(0);
 
    
  }
  
  if(i == loops/2 + 4){
    //analog_amps = analogRead(amps_pin);
    analog_amps2 = ads.readADC_SingleEnded(3);
  }

  i++;
  if(i == loops){
    Serial.print(analog_voltage1);
    Serial.print(" ");    
    Serial.println(analog_amps1);
    
    /*Serial.print(analog_voltage1);
    Serial.print(" ");
    Serial.print(analog_amps1);
    Serial.print(" ");
    Serial.print(analog_voltage2);
    Serial.print(" ");
    Serial.println(analog_amps2);
    */
    i = 0;
  }
}
