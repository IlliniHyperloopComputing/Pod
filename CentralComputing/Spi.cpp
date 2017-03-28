#include "Spi.h"
#include "Utils.h"

Spi::Spi(Xmega_Setup * x1, Xmega_Setup * x2){
  //Store Xmega_Setup
  this->x1 = x1;
  this->x2 = x2;

  //Dynamically allocate buffer for each Xmega
  //2 extra bytes for X_SENSOR_STATUS and X_STATE
  //Is now large enough to fit X_ALL in the buffer
  //2 extra bytes for 16-bit CRC checksum
  this->x1_buff = new uint8_t [x1->num_items + 2 + 2];
  this->x2_buff = new uint8_t [x2->num_items + 2 + 2];

  //Calculate offset_lookup table.
  this->x1_offset_lookup = new uint8_t [x1->num_items];
  this->x2_offset_lookup = new uint8_t [x2->num_items];
  
  x1_offset_lookup[0] = 0;
  for(int i=1; i<x1->num_items; i++){
    x1_offset_lookup[i] = x1_offset_lookup[i-1] + x1->bytes_per_item[i-1];
  }

  x2_offset_lookup[0] = 0;
  for(int i=1; i<x2->num_items; i++){
    x2_offset_lookup[i] = x2_offset_lookup[i-1] + x2->bytes_per_item[i-1];
  }

  


}

Spi::~Spi(){

}

