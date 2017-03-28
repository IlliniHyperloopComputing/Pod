#include "Spi.h"
#include "Utils.h"

Spi::Spi(Xmega_Setup * x1, Xmega_Setup * x2){
  print_debug("Entering SPI constructor\n");

  //Store Xmega_Setup
  this->x1 = x1;
  this->x2 = x2;

  //Create offset_lookup tables
  this->x1_offset_lookup = new uint8_t [x1->num_items];
  this->x2_offset_lookup = new uint8_t [x2->num_items];

  //Calculate offset_lookup table.
  //Calculate total num_bytes in each data buffer
  assert(x2->bytes_per_item[0] <= 4); //Assert bytes_per_index is at most 4
  x1_num_bytes = x1->bytes_per_item[0];
  x1_offset_lookup[0] = 0;
  for(int i=1; i<x1->num_items; i++){
    assert(x1->bytes_per_item[i] <= 4); //Assert bytes_per_index is at most 4
    x1_num_bytes = x1->bytes_per_item[i];
    x1_offset_lookup[i] = x1_offset_lookup[i-1] + x1->bytes_per_item[i-1];
  }

  assert(x2->bytes_per_item[0] <= 4); //Assert bytes_per_index is at most 4
  x2_num_bytes = x2->bytes_per_item[0];
  x2_offset_lookup[0] = 0;
  for(int i=1; i<x2->num_items; i++){
    assert(x2->bytes_per_item[i] <= 4); //Assert bytes_per_index is at most 4
    x2_num_bytes = x2->bytes_per_item[i];
    x2_offset_lookup[i] = x2_offset_lookup[i-1] + x2->bytes_per_item[i-1];
  }

  //Dynamically allocate buffer for each Xmega
  //2 extra bytes for X_SENSOR_STATUS and X_STATE
  //Is now large enough to fit X_ALL in the buffer
  //2 extra bytes for 16-bit CRC checksum
  this->x1_buff = new uint8_t [x1_num_bytes + 2 + 2];
  this->x2_buff = new uint8_t [x2_num_bytes + 2 + 2];

  //Setup SPI on each Xmega
  setup_spi();

  print_debug("Exiting SPI Constructor\n");

}

Spi::~Spi(){
  //Free data buffers
  if(this->x1_buff){
    delete[] this->x1_buff;
  }
  if(this->x2_buff){
    delete[] this->x2_buff;
  }

  //Free offset lookup tables
  if(this->x1_offset_lookup){
    delete[] this->x1_offset_lookup;
  }
  if(this->x2_offset_lookup){
    delete[] this->x2_offset_lookup;
  }

}

void Spi::request(Xmega_Request &request_type){

}

uint32_t Spi::get_data(uint8_t device, int idx){
  //Create pointers to actual structures
  uint8_t * buff = NULL;
  uint8_t * bpi = NULL;
  uint8_t * offset = NULL;
  uint8_t max_size = 0;

  //Select appropriate device, and fill pointers
  if(device == 0){
    buff = x1_buff;
    bpi = x1->bytes_per_item;
    offset = x1_offset_lookup;
    max_size = x1_num_bytes;

    //assert to ensure no buffer overflow
    assert(idx < x1->num_items);
  }
  else if(device == 1){
    buff = x2_buff;
    bpi = x2->bytes_per_item;
    offset = x2_offset_lookup;
    max_size = x2_num_bytes;

    //assert to ensure no buffer overflow
    assert(idx < x2->num_items);
  }
  else{
    //If a device number other than 0 or 1 is given
    return 0;
  }

  //Pull the correct data
  uint32_t tmp = 0;
  for(int i = 0; i< bpi[idx] ; i++){
    //Assert to ensure no buffer overflow
    assert(offset[idx] + i < max_size);
    tmp |= buff[offset[idx] + i] << (i * 8);
  }

  return tmp;
}

uint8_t Spi::get_sensor_status(uint8_t device){
  if(device == 0){
    return x1_sensor_status; 
  }
  else if(device == 1){
    return x2_sensor_status; 
  }

  return 0;
}

uint8_t Spi::get_state(uint8_t device){
  if(device == 0){
    return x1_state;  
  }
  else if(device == 1){
    return x2_state;  
  }

  return 0;
}

int Spi::setup_spi(){

  //Open files
  print_debug("Opening Xmega1 file %s ... ",x1->file_path);
	fd1 = open(x1->file_path, O_RDWR);
	if (fd1 < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  print_debug("Opening Xmega2 file %s ... ",x2->file_path);
	fd2 = open(x2->file_path, O_RDWR);
	if (fd2 < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  //Set speed
  print_debug("Setting Xmega1 speed %d ... ",x1->speed);
	if (ioctl(fd1, SPI_IOC_WR_MAX_SPEED_HZ, &(x1->speed)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  print_debug("Setting Xmega2 speed %d ... ",x2->speed);
	if (ioctl(fd2, SPI_IOC_WR_MAX_SPEED_HZ, &(x2->speed)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  //Set bits per word
  print_debug("Setting Xmega1 bits per word %d ... ",x1->bits_per_word);
	if (ioctl(fd1, SPI_IOC_WR_BITS_PER_WORD, &(x1->bits_per_word)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  print_debug("Setting Xmega2 bits per word %d ... ",x2->bits_per_word);
	if (ioctl(fd2, SPI_IOC_WR_BITS_PER_WORD, &(x2->bits_per_word)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  return 0;
}
