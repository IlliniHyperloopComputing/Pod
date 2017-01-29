#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads; 
Adafruit_ADS1115 ads1(0x49);/* Use this for the 16-bit version */
Adafruit_ADS1115 ads2(0x4a);
Adafruit_ADS1115 ads3(0x4b);


void setup(void) 
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
   ads1.setGain(GAIN_ONE); 
   ads2.setGain(GAIN_ONE);
   ads3.setGain(GAIN_ONE);
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();
  //ads1.begin();
  ads1.begin();
  ads2.begin();
  ads3.begin();
  Wire.begin(0x20);
  Wire.onRequest(beagleTransmit);
  Wire.onReceive(beagleReceive);
}

int16_t adc0[4];
int16_t adc1[4];
int16_t adc2[4];
int16_t adc3[4];


uint8_t which = 0;
void beagleReceive(int numbytes){
  which = Wire.read();
  Serial.println(which);
}
void beagleTransmit() {
  int16_t x = 0;
  if(which<4){
    x = adc0[which%4];
  } else if(which<8){
    //Serial.println("yoo");
    x = adc1[which%4];
    //x = 5000;
  } else if(which<12){
    x = adc2[which%4];
  } else if(which<16){
    x = adc3[which%4];
  }
  Wire.write(x);
  Wire.write(x >> 8);
}

void loop(void) 
{

//ACCELEROMETERS  
  adc0[0] = ads.readADC_SingleEnded(0);
  adc0[1] = ads.readADC_SingleEnded(1);
  adc0[2] = ads.readADC_SingleEnded(2);
//ACCELEROMETERS END

  //adc0[3] = ads.readADC_SingleEnded(3);

//DISTANCE
  adc1[0] = ads1.readADC_SingleEnded(0);
  adc1[1] = ads1.readADC_SingleEnded(1);
  adc1[2] = ads1.readADC_SingleEnded(2);
//END DISTANCE

  //adc1[3] = ads1.readADC_SingleEnded(3);

  /*adc2[0] = ads1.readADC_SingleEnded(0);
  adc2[1] = ads1.readADC_SingleEnded(1);
  adc2[2] = ads1.readADC_SingleEnded(2);
  adc2[3] = ads1.readADC_SingleEnded(3);

  adc3[0] = ads1.readADC_SingleEnded(0);
  adc3[1] = ads1.readADC_SingleEnded(1);
  adc3[2] = ads1.readADC_SingleEnded(2);
  adc3[3] = ads1.readADC_SingleEnded(3);
  */

  //Serial.println(adc0[0]);
  delay(10);
  
}
