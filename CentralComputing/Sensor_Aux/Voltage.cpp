#include "Voltage.h"

#define TAPE_DISTANCE 30.48 
#define ENCODER_RADIUS 0.0635
#define SECONDS 60 // would this be defined as 60 

#define RPM_OFFSET 0


using namespace std; 





Arbitrary_Data voltage_calculation(Arbitrary_Data raw) {
  Voltage_Raw v_raw = *(Voltage_Raw *)raw.data;                           // Convert the raw data into a useable struct								
  Voltage_Calc * v_calc = (Voltage_Calc *) malloc(sizeof(Voltage_Calc)); 
  v_calc->voltage = final_voltage;                                                  
  Arbitrary_Data calculated;                                                //Create a new arbitrary data struct
  calculated.size = sizeof(Voltage_Calc);                                  //Set the size of the calculated data
  calculated.data = (uint16_t *)v_calc;                                      //Make the data pointer point to our allocated struct
  return calculated;                                                        //Copy out calculated to the calling function
}

void velocity_parse(Spi * spi, Arbitrary_Data raw) {
  Voltage_Raw * v_raw = (Voltage_Raw *)raw.data;                           // Convert the raw data into a useable struct
  
}
