#include "Distance.h"


#define TAPE_DISTANCE 30.48
#define ENCODER_RADIUS 0.0635
#define SECONDS 60

#define TAPE_OFFSET 0
#define ENCODER_COUNT_OFFSET 0 //TODO get actual values for buffer reading, keep within another file

using namespace std; 

Arbitrary_Data distance_calculation(Arbitrary_Data raw) {
  Distance_Raw d_raw = *(Distance_Raw *)raw.data;                           // Convert the raw data into a useable struct
  double tape_count_distance = d_raw.tape_count * TAPE_DISTANCE;            // Convert the tape counts into meters traveled
  double encoder_count_distance = d_raw.encoder_count * ENCODER_RADIUS * 2 * M_PI;// Convert the encoder count into meters
  double best = max(tape_count_distance, encoder_count_distance);           // Grab the max of the two values as the real value
  Distance_Calc * d_calc = (Distance_Calc *) malloc(sizeof(Distance_Calc)); // Allocate space for the calculated object
  d_calc->distance = best;                                                  //place the max value in there
  Arbitrary_Data calculated;                                                //Create a new arbitrary data struct
  calculated.size = sizeof(Distance_Calc);                                  //Set the size of the calculated data
  calculated.data = (uint8_t *)d_calc;                                      //Make the data pointer point to our allocated struct
  return calculated;                                                        //Copy out calculated to the calling function
}

void distance_parse(Spi * spi, Arbitrary_Data raw) {
  Distance_Raw * d_raw = (Distance_Raw *)raw.data;                           // Convert the raw data into a useable struct
  d_raw->tape_count = spi->get_data(XMEGA2, TAPE_COUNT_INDEX);
  d_raw->encoder_count = spi->get_data(XMEGA1, ENCODER_COUNT_INDEX);
}


