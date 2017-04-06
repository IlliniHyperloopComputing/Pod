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
    x1_num_bytes += x1->bytes_per_item[i];
    x1_offset_lookup[i] = x1_offset_lookup[i-1] + x1->bytes_per_item[i-1];
  }

  assert(x2->bytes_per_item[0] <= 4); //Assert bytes_per_index is at most 4
  x2_num_bytes = x2->bytes_per_item[0];
  x2_offset_lookup[0] = 0;
  for(int i=1; i<x2->num_items; i++){
    assert(x2->bytes_per_item[i] <= 4); //Assert bytes_per_index is at most 4
    x2_num_bytes += x2->bytes_per_item[i];
    x2_offset_lookup[i] = x2_offset_lookup[i-1] + x2->bytes_per_item[i-1];
  }

  //Dynamically allocate buffer for each Xmega
  //2 extra bytes for X_SENSOR_STATUS and X_STATE
  //Is now large enough to fit X_ALL in the buffer
  //2 extra bytes for 16-bit CRC checksum
  this->x1_buff = new uint8_t [x1_num_bytes + 2 + 2];
  this->x2_buff = new uint8_t [x2_num_bytes + 2 + 2];

  //Setup SPI on each Xmega
  if(setup_spi()){
    print_debug("Failure setting up SPI\n");
    assert(false);
  }

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

void Spi::transfer(Xmega_Transfer &xt){

  //Create command/transmission buffers
  uint8_t tx1_buff[] = {0xAA, (uint8_t)xt.cmd1, (uint8_t)xt.req1, 0,0};
  uint8_t tx2_buff[] = {0xAA, (uint8_t)xt.cmd2, (uint8_t)xt.req2, 0,0};

  //Calculate CRC
  uint16_t crc1 = Crc::CRCCCITT(tx1_buff, 3, 0);
  uint16_t crc2 = Crc::CRCCCITT(tx2_buff, 3, 0);

  //Create variables for when read() is called to confrim proper transmission
  uint8_t result1 = 0;
  uint8_t result2 = 0;

  //Store CRC
  tx1_buff[3] = (uint8_t) crc1;
  tx1_buff[4] = (uint8_t) (crc1>>8);
  tx2_buff[3] = (uint8_t) crc2;
  tx2_buff[4] = (uint8_t) (crc2>>8);

  int sent_properly = 0;
  int send_type = 0;
  int max_passes = 2;
  int passes = 0;
  while(!sent_properly && passes < max_passes){
    print_debug("Send Loop: pass #%d, send_type: %d\n",passes,send_type);
    //send to Xmegas
    for(int i = 0; i < 5;i++){
      //Why equal 0 or 3? check below where send_type is set
      if(send_type == 0 || send_type == 0b11){
        write(fd1, tx1_buff + i, 1);
        write(fd2, tx2_buff + i, 1);
      }
      else if(send_type == 1){
        write(fd1, tx1_buff + i, 1);
        usleep(10);
      }
      else{
        write(fd2, tx2_buff + i, 1);
        usleep(10);
      }
    }

    //read ACK or NACK from Xmegas to confirm sent data properly
    if(send_type == 0){
      read(fd1, &result1, 1);
      read(fd2, &result2, 1);
    }
    else if(send_type == 1){
      read(fd1, &result1, 1);
    }
    else{
      read(fd2, &result2, 1);
    }

    //Determine if another attempt is needed
    sent_properly = (result1 == CRC_PASS) && (result2 == CRC_PASS);
    //send_type is used to say which one we will try to resend.
    //0b01 => resend xmega1, 0b10 => resend xmega2, 0b11 => resend xmega2
    send_type = (result1 != CRC_PASS) | ((result2 != CRC_PASS) << 1);
    
    print_debug("Send Loop: result1: %d\n", result1);
    print_debug("Send Loop: result2: %d\n", result2);
    passes++;
  }
  
  //used to set up recieve_type in data loops below
  //This way, we can continue to read in X1 even if
  //X2 has already failed. We just skip X2
  //Look for the initialization of recieve_type
  int sending_errors = 0; 

  //Set appropriate error messages
  if(!sent_properly){
    if((send_type & 0b01)){
      print_debug("Transmission_failure in X1 when sending \n");
      x1_transmission_failure = X_TF_SEND;
    }
    if((send_type & 0b10)){
      print_debug("Transmission_failure in X2 when sending \n");
      x2_transmission_failure = X_TF_SEND;
    }
    sending_errors = send_type;
  }

  //Now to read in all data
  
  //Setup size of data to read in
  uint8_t bytes_to_read1 = 0;
  uint8_t bytes_to_read2 = 0;
  
  if(xt.req1 == 0){
    bytes_to_read1 = x1_num_bytes;
  }
  else if(xt.req1 == 1 || xt.req1 ==2){
    bytes_to_read1 = 1;
  }
  else if(xt.req1 == 3){
    bytes_to_read1 = x1_num_bytes+2;
  }
  else{
    bytes_to_read1 = 0;
  }

  print_debug("Bytes_to_read1: %d\n", bytes_to_read1);

  if(xt.req2 == 0){
    bytes_to_read2 = x2_num_bytes;
  }
  else if(xt.req2 == 1 || xt.req2 ==2){
    bytes_to_read2 = 1;
  }
  else if(xt.req2 == 3){
    bytes_to_read2 = x2_num_bytes+2;
  }
  else{
    bytes_to_read2 = 0;
  }
  print_debug("Bytes_to_read2: %d\n", bytes_to_read2);

  //Create buffers that are big enough for size of incoming data
  uint8_t rx1_buff[bytes_to_read1+2];
  uint8_t rx2_buff[bytes_to_read2+2];
  memset(&rx1_buff, 0, bytes_to_read1+2);
  memset(&rx2_buff, 0, bytes_to_read2+2);
  //Used to store CRC calculation 
  uint16_t rx_crc1 = 0;
  uint16_t rx_crc2 = 0;
  uint16_t calc_crc1 = 0; 
  uint16_t calc_crc2 = 0; 

  //Used for retry purposes
  passes = 0;
  int recieved_properly = 0;
  int recieve_type = sending_errors;
  while(!recieved_properly && passes < 2){
    print_debug("Recieve Loop: pass #%d, recieve_type: %d\n",passes,recieve_type);

    //Why separate loops for reading data and crc?
    //Why not just one loop that goes two extra times?
    //Because NONE is a request option, and NO data should be read
    //To implement the one loop solution, just increment
    //bytes_to_read* +2 and there you go. 
    //However, I think its simpler to just have two loops

    //Read data
    uint8_t idx1 = 0;
    uint8_t idx2 = 0;
    while(idx1 < bytes_to_read1 || idx2 < bytes_to_read2 ){
      if((idx1 < bytes_to_read1 && idx2 < bytes_to_read2) && (recieve_type == 0 || recieve_type == 3)){ //read both
        read(fd1, rx1_buff+idx1, 1);
        read(fd2, rx2_buff+idx2, 1);
        print_debug("Recieved x1: idx: %d, data: %d\n", idx1, rx1_buff[idx1] );
        print_debug("Recieved x2: idx: %d, data: %d\n", idx2, rx2_buff[idx2] );
      }
      else if((idx1 < bytes_to_read1) && recieve_type != 2){//read fd1
        read(fd1, rx1_buff+idx1, 1);
        print_debug("Recieved x1: idx: %d, data: %d\n", idx1, rx1_buff[idx1] );
        usleep(10);
      }
      else if((idx2 < bytes_to_read2) && recieve_type != 1){//read fd2
        read(fd2, rx2_buff+idx2, 1);
        print_debug("Recieved x2: idx: %d, data: %d\n", idx2, rx2_buff[idx2] );
        usleep(10);
      }
      idx1++;
      idx2++;
    }

    calc_crc1 = Crc::CRCCCITT(rx1_buff, bytes_to_read1, 0);
    calc_crc2 = Crc::CRCCCITT(rx1_buff, bytes_to_read2, 0);
    print_debug("Calc x1 crc: data: 0x%x\n", calc_crc1);
    print_debug("Calc x2 crc: data: 0x%x\n", calc_crc2);

    //Read CRC
    uint8_t idx_crc1 = 0;
    uint8_t idx_crc2 = 0;
    rx_crc1 = 0;
    rx_crc2 = 0;
    while(idx_crc1 < 2 || idx_crc2 < 2){
      if((bytes_to_read1 != 0 && bytes_to_read2 != 0) && (recieve_type == 0 || recieve_type == 3)){
        read(fd1, rx1_buff+ bytes_to_read1 + idx_crc1, 1);
        read(fd2, rx2_buff+ bytes_to_read2 + idx_crc2, 1);
        rx_crc1 |= rx1_buff[bytes_to_read1 + idx_crc1] << (idx_crc1 * 8);
        rx_crc2 |= rx2_buff[bytes_to_read2 + idx_crc2] << (idx_crc2 * 8);
        print_debug("Recieved x1 crc: data: 0x%x\n", rx1_buff[idx1+idx_crc1]);
        print_debug("Recieved x2 crc: data: 0x%x\n", rx2_buff[idx2+idx_crc2]);
      }
      else if(bytes_to_read1 != 0 && recieve_type != 2){
        read(fd1, rx1_buff+ bytes_to_read1 + idx_crc1, 1);
        rx_crc1 |= rx1_buff[bytes_to_read1 + idx_crc1] << (idx_crc1 * 8);
        print_debug("Recieved x1 crc: data: 0x%x\n", rx1_buff[idx1+idx_crc1]);
        usleep(10);
      }
      else if(bytes_to_read2 != 0 && recieve_type != 1){
        read(fd2, rx2_buff+ bytes_to_read2 + idx_crc2, 1);
        rx_crc2 |= rx2_buff[bytes_to_read2 + idx_crc2] << (idx_crc2 * 8);
        print_debug("Recieved x2 crc: data: 0x%x\n", rx2_buff[idx2+idx_crc2]);
        usleep(10);
      }
      idx_crc1++;
      idx_crc2++;
    }

    print_debug("Rec x1 crc: data: 0x%x\n", rx_crc1);
    print_debug("Rec x2 crc: data: 0x%x\n", rx_crc2);

    //Determine if another attempt is needed
    recieved_properly = (calc_crc1 == rx_crc1) && (calc_crc2 == rx_crc2);
    //recieve_type is used to say which one we will try to re-recieve
    //works like above send_type
    recieve_type = (calc_crc1 != rx_crc1) | ((calc_crc2 != rx_crc2) << 1);
    passes++;
  }

  if(!recieved_properly){
    if((recieve_type & 0b01) && x1_transmission_failure == 0){
      x1_transmission_failure = X_TF_RECIEVE;
      print_debug("Transmission_failure in X1 when recieving \n");
    }
    if((send_type & 0b10) && x2_transmission_failure == 0){
      x2_transmission_failure = X_TF_RECIEVE;
      print_debug("Transmission_failure in X2 when recieving \n");
    }
    return;
  }

  //Copy recieved data!
  if(recieve_type == 0){
    memcpy(x1_buff, &rx1_buff, bytes_to_read1);
    memcpy(x2_buff, &rx2_buff, bytes_to_read2);
    //Set appropriate sensor_state or state
    if(xt.req1 == 1){
      x1_sensor_status = rx1_buff[bytes_to_read1];
    }
    else if(xt.req1 ==2){
      x1_state = rx1_buff[bytes_to_read1];
    }
    else{
      x1_sensor_status = rx1_buff[bytes_to_read1];
      x1_state = rx1_buff[bytes_to_read1+1];
    }

    //Set appropriate sensor_state or state
    if(xt.req2 == 1){
      x2_sensor_status = rx2_buff[bytes_to_read2];
    }
    else if(xt.req2 ==2){
      x2_state = rx2_buff[bytes_to_read2];
    }
    else{
      x2_sensor_status = rx2_buff[bytes_to_read2];
      x2_state = rx2_buff[bytes_to_read2+1];
    }
  }
  else if(recieve_type == 1){//Xmega1 failed. 
    memcpy(x2_buff, &rx2_buff, bytes_to_read2);
    //Set appropriate sensor_state or state
    if(xt.req2 == 1){
      x2_sensor_status = rx2_buff[bytes_to_read2];
    }
    else if(xt.req2 ==2){
      x2_state = rx2_buff[bytes_to_read2];
    }
    else{
      x2_sensor_status = rx2_buff[bytes_to_read2];
      x2_state = rx2_buff[bytes_to_read2+1];
    }
  }
  else if(recieve_type == 2){//Xmega2 failed.
    memcpy(x1_buff, &rx1_buff, bytes_to_read1);
    //Set appropriate sensor_state or state
    if(xt.req1 == 1){
      x1_sensor_status = rx1_buff[bytes_to_read1];
    }
    else if(xt.req1 ==2){
      x1_state = rx1_buff[bytes_to_read1];
    }
    else{
      x1_sensor_status = rx1_buff[bytes_to_read1];
      x1_state = rx1_buff[bytes_to_read1+1];
    }
  }
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
  
  //Check if the device tree has been loaded
  print_debug("Checking Device tree for SPI initialization\n");
  int spi_device_tree = system("grep SPI < /sys/devices/platform/bone_capemgr/slots");
  print_debug("Exit value from grep: %d\n",spi_device_tree);
  if(spi_device_tree != 0){
    print_debug("Echo-ing correct device tree setup\n");
    system("echo BB-SPI-M-01 > /sys/devices/platform/bone_capemgr/slots");
    print_debug("Sleeping for 2 seconds to make sure kernel has time complete setup\n");
    usleep(2000000);
  }

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
