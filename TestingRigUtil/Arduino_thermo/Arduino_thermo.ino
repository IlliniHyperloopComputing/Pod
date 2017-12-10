#include <Wire.h>

#include <Adafruit_MAX31855.h>

/*************************************************** 
  This is an example for the Adafruit Thermocouple Sensor w/MAX31855K

  Designed specifically to work with the Adafruit Thermocouple Sensor
  ----> https://www.adafruit.com/products/269

  These displays use SPI to communicate, 3 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <SPI.h>

// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
//#define MAXDO   3
//#define MAXCS   4
//#define MAXCLK  5

// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
Adafruit_MAX31855 thermo1(10);
Adafruit_MAX31855 thermo2(8);

void setup() {
  Serial.begin(115200);  
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
    
   /*double c1 = thermo1.readCelsius();
   Serial.println(c1);*/
   
   double c1 = thermo1.readCelsius();
   double c2 = thermo2.readCelsius();
   Serial.print(c1);
   Serial.print(" ");
   Serial.println(c2);
   
   
   
 
   delay(250);
}
