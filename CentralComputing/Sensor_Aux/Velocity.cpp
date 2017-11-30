#include "Velocity.h"

#define TAPE_DISTANCE 30.48 
#define ENCODER_RADIUS 0.0635
#define SECONDS 60 // would this be defined as 60 

#define RPM_OFFSET 0


using namespace std; 





Arbitrary_Data velocity_calculation(Arbitrary_Data raw) {
  Velocity_Raw v_raw = *(Velocity_Raw *)raw.data;                           // Convert the raw data into a useable struct
  double rpm = v_raw.rpm;	
  double final_rpm = rpm / SECONDS * 2 * M_PI * ENCODER_RADIUS;					//final velocity calculation --> rpm into seconds				
  Velocity_Calc * v_calc = (Velocity_Calc *) malloc(sizeof(Velocity_Calc)); //space for the calculated veloctiy-> 8 bits
  v_calc->velocity = final_rpm;                                                  
  Arbitrary_Data calculated;                                                //Create a new arbitrary data struct
  calculated.size = sizeof(Velocity_Calc);                                  //Set the size of the calculated data
  calculated.data = (uint8_t *)v_calc;                                      //Make the data pointer point to our allocated struct
  return calculated;                                                        //Copy out calculated to the calling function
}

void velocity_parse(Spi * spi, Arbitrary_Data raw) {
  Velocity_Raw * v_raw = (Velocity_Raw *)raw.data;                           // Convert the raw data into a useable struct
  v_raw->rpm = spi->get_data(XMEGA1, ENCODER_RPM_INDEX);
  
}
