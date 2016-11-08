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
#include <Wire.h>
#include <Adafruit_MAX31855.h>


// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:


// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO   3 // MAXDO on thermocouples all connect to pin 3
#define MAXCLK  5 // CLK on all thermocouples all connect to pin 5
#define NUM_THERMO  8 // Number of thermocouples
//
int MAXCS[] = {0,4,8,9,10,11,12,13}; // CS on thermocouples connect to these pins, respectively
byte temps[NUM_THERMO]; // Initializing array to hold temperature values in byte format




// Declaring the thermocouples
Adafruit_MAX31855 * thermocouples[NUM_THERMO];


// Example creating a thermocouple instance with hardware SPI (Uno/Mega only)
// on a given CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS);


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

void setup() {
#ifndef ESP8266
//  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Wire.begin(0x15);
  Serial.begin(9600);


  Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);
  for (int i = 0; i < NUM_THERMO; i++) {
    thermocouples[i] = new Adafruit_MAX31855(MAXCLK, MAXCS[i], MAXDO); // Make all the new thermocouple objects
  }
  Wire.onRequest(beagleLoop); // Arduino writes data when interrupted by Beaglebone(runs beagleLoop code)
  Wire.onReceive(beagleReceive);
 
}

uint8_t which =0;

void beagleReceive(int numBytes){
  which = Wire.read();
}

void beagleLoop() {
  Wire.write(temps[which]);
  /*if(which == 1){
    Wire.write(12);
    return;
  }
  Wire.write(1);
  Serial.print(which);
  if(which<8 && which >=0){
    Wire.write(temps[which]);
  }
  */
  //Wire.write(temps, NUM_THERMO); // Writing the data to the arduino  
}


void loop() {
  for (int i = 0; i < NUM_THERMO; i++) {
    if (isnan(thermocouples[i]->readFarenheit())) {
      Serial.print("Something is wrong with thermocouple ");
      Serial.print(i);
    }
    else {
      temps[i] = byte(thermocouples[i]->readFarenheit()); // cast double output of method readFarenheit to bytes
      Serial.print(i);
      Serial.print(" F = ");
      Serial.println(thermocouples[i]->readFarenheit());
    }
  }
 
  //Serial.print("Distance = ");
  //Serial.println(distance);
  delay(100);
  
}
