Tachometer.cpp

#include "Tachometer.h"

#define TAPE_DISTANCE 30.48 
#define ENCODER_RADIUS 0.0635
#define SECONDS 60 

#define RPM_OFFSET 0


using namespace std; 


Arbitrary_Data tachometer_calculation(Arbitrary_Data raw) {
  Tachometer_Raw t_raw = *(Tachometer_Raw *)raw.data;                           // Convert the raw data into a useable struct
  double rpm = t_raw.rpm;  
  Tachometer_Calc * t_calc = (Tachometer_Calc *) malloc(sizeof(Tachometer_Calc)); //space for the calculated tachometer-> 8 bits
  t_calc->rpm = final_rpm;                                                  
  Arbitrary_Data calculated;                                                //Create a new arbitrary data struct
  calculated.size = sizeof(Tachometer_Calc);                                  //Set the size of the calculated data
  calculated.data = (uint8_t *)t_calc;                                      //Make the data pointer point to our allocated struct
  return calculated;                                                        //Copy out calculated to the calling function
}

void tachometer_parse(uint8_t * buffer, Arbitrary_Data raw) {
  Tachometer_Raw * t_raw = (Tachometer_Raw *)raw.data;                           // Convert the raw data into a useable struct
  // These are placeholders for now. They need to be updated with real values
  t_raw->rpm = (int) *(buffer + RPM_OFFSET); 
