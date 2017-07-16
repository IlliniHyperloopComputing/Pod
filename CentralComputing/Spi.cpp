#include "Spi.h"
#include "Utils.h"

Spi::Spi(Xmega_Setup * x1s, Xmega_Setup * x2s){
  print_debug("Entering SPI constructor\n");

  //Store Xmega_Setup
  x1_setup = x1s;
  x2_setup = x2s;

  //Create Xmega_Data
  x1 = new Xmega_Data;
  x2 = new Xmega_Data;

  //Create offset_lookup tables
  x1->offset_lookup = new uint8_t [x1_setup->num_items];
  x2->offset_lookup = new uint8_t [x2_setup->num_items];

  //Calculate offset_lookup table.
  //Calculate total num_bytes in each data buffer
  setup_offset(x1, x1_setup);
  setup_offset(x2, x2_setup);

  //Dynamically allocate buffer for each Xmega
  //2 extra bytes for X_SENSOR_STATUS and X_STATE
  //Is now large enough to fit X_ALL in the buffer
  //2 extra bytes for 16-bit CRC checksum
  x1->buff = new uint8_t [x1->num_bytes + 2 + 2];
  x2->buff = new uint8_t [x2->num_bytes + 2 + 2];

  //Setup SPI on each Xmega
  if(setup_spi()){
    print_debug("Failure setting up SPI\n");
    assert(false);
  }

  print_debug("Exiting SPI Constructor\n");
}

Spi::~Spi(){
  //Free data buffers
  if(this->x1->buff){
    delete[] this->x1->buff;
  }
  if(this->x2->buff){
    delete[] this->x2->buff;
  }

  //Free offset lookup tables
  if(this->x1->offset_lookup){
    delete[] this->x1->offset_lookup;
  }
  if(this->x2->offset_lookup){
    delete[] this->x2->offset_lookup;
  }

  delete x1;
  delete x2;

}

int Spi::transfer(Xmega_Transfer &xt){

  Xmega_Data * xd;
  if(xt.device == 0){
    xd = x1;
  }
  else{
    xd = x2;
  }

  //Create command/transmission buffers
  uint8_t tx_buff[] = {0xAA, xt.cmd, xt.req, 0,0};

  //Calculate CRC
  uint16_t send_crc = Crc::CRCCCITT(tx_buff, 3, 0);

  //Create variables for when read() is called to confrim proper transmission
  uint8_t send_result = 0;

  //Store CRC
  tx_buff[3] = (uint8_t) send_crc;
  tx_buff[4] = (uint8_t) (send_crc>>8);

  int sent_properly = 0;
  int max_passes = 3;
  int passes = 0;
  while(!sent_properly && passes < max_passes){
    if(passes > 0 || send_result == 0x4A){
      print_debug("Sending to Xmega: passes: %d send_properly: %d\n", passes, sent_properly);
      usleep(SLEEP_TIME*100);
    }
    //send to Xmegas
    print_debug("wval: \t");
    for(int i = 0; i < 5;i++){
      usleep(SLEEP_TIME);
      int wval = write(xd->fd, tx_buff + i, 1);
      print_debug("%d, tx: %x\t", wval, tx_buff[i]);
    }
    print_debug("\n");

    //read ACK or NACK from Xmegas to confirm sent data properly
    usleep(SLEEP_TIME*10);

    int rval = read(xd->fd, &send_result, 1);
    print_debug("rval: %d\n", rval);
    print_debug("read: %x\n", send_result);
    

    //Determine if another attempt is needed
    sent_properly = (send_result == CRC_PASS) ;

    passes++;
  }
  
  //return if not sent properly
  if(!sent_properly){
    return 1;
  }

  if(xt.req == X_R_NONE){
    return 0;
  }
  
  //Setup size of data to read in
  uint8_t bytes_to_read = 0;
  
  if(xt.req == X_R_SENSOR){
    bytes_to_read = xd->num_bytes;
  }
  else if(xt.req == X_R_SENSOR_STATUS || xt.req == X_R_STATE){
    bytes_to_read = 1;
  }
  else if(xt.req == X_R_ALL){
    bytes_to_read = xd->num_bytes+2;
  }
  else{
    bytes_to_read = 0;
  }

  //Read data
  //Reading happens once. If it fails, then it failes

  //Why separate loops for reading data and crc?
  //Why not just one loop that goes two extra times?
  //Because NONE is a request option, and NO data should be read
  //To implement the one loop solution, just increment
  //bytes_to_read* +2 and there you go. But you need aditional ifs
  //However, I think its simpler to just have two loops

  //Create buffers that are big enough for size of incoming data
  uint8_t rx_buff[bytes_to_read+2];
  memset(&rx_buff, 0, bytes_to_read+2);
  uint8_t idx = 0;
  while(idx < bytes_to_read ){
    usleep(SLEEP_TIME);
    read(xd->fd, rx_buff+idx, 1);
    print_test("Reading: idx:%d\t, rx_buff: %x\n",idx, rx_buff[idx]);
    idx++;
  }

  //usleep(SLEEP_TIME *10);

  //Calculate CRC
  uint16_t calc_crc = 0; 
  calc_crc = Crc::CRCCCITT(rx_buff, bytes_to_read, 0);
  print_test("Calc x1 crc: data: 0x%x\n", calc_crc);

  //Used to store incoming CRC
  uint16_t rx_crc = 0;
  uint8_t idx_crc = 0;

  //Read CRC
  while(idx_crc < 2) {
    usleep(SLEEP_TIME);
    read(xd->fd, rx_buff+ bytes_to_read + idx_crc, 1);
    rx_crc |= rx_buff[bytes_to_read + idx_crc] << (idx_crc * 8);
    idx_crc++;
    print_test("crc: data: 0x%x\n", rx_crc);
  }

  //Check CRC accuracy 
  //Return if they are not equal
  if(calc_crc != rx_crc){
    return 2;
  }

  //Copy in the data!
  if(xt.req == X_R_SENSOR || xt.req == X_R_ALL){
    memcpy(xd->buff, &rx_buff, bytes_to_read);
  }

  //Set appropriate sensor_state or state
  if(xt.req == X_R_SENSOR_STATUS){
    xd->sensor_status = rx_buff[0];
  }
  else if(xt.req == X_R_STATE){
    xd->state = rx_buff[0];
  }
  else if(xt.req == X_R_ALL){
    xd->sensor_status = rx_buff[bytes_to_read-2];
    xd->state = rx_buff[bytes_to_read-1];
  }

  return 0;
}

uint32_t Spi::get_data(uint8_t device, int idx){
  Xmega_Data * xd;
  Xmega_Setup * xs;
  if(device == 0){
    xd = x1;
    xs = x1_setup;
  }
  else{
    xd = x2;
    xs = x2_setup;
  }

  assert(idx < xs->num_items);

  //Pull the correct data
  uint32_t tmp = 0;
  for(int i = 0; i< xs->bytes_per_item[idx] ; i++){
    //Assert to ensure no buffer overflow
    assert(xd->offset_lookup[idx] + i < xd->num_bytes);
    tmp |= xd->buff[xd->offset_lookup[idx] + i] << (i * 8);
  }

  return tmp;
}

uint8_t Spi::get_sensor_status(uint8_t device){
  if(device == 0){
    return x1->sensor_status; 
  }
  else if(device == 1){
    return x2->sensor_status; 
  }

  return 0;
}

uint8_t Spi::get_state(uint8_t device){
  if(device == 0){
    return x1->state;  
  }
  else if(device == 1){
    return x2->state;  
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
  print_debug("Opening Xmega1 file %s ... ",x1_setup->file_path);
	x1->fd = open(x1_setup->file_path, O_RDWR);
	if (x1->fd < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  print_debug("Opening Xmega2 file %s ... ",x2_setup->file_path);
	x2->fd = open(x2_setup->file_path, O_RDWR);
	if (x2->fd < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  //Set speed
  print_debug("Setting Xmega1 speed %d ... ",x1_setup->speed);
	if (ioctl(x1->fd, SPI_IOC_WR_MAX_SPEED_HZ, &(x1_setup->speed)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  print_debug("Setting Xmega2 speed %d ... ",x2_setup->speed);
	if (ioctl(x2->fd, SPI_IOC_WR_MAX_SPEED_HZ, &(x2_setup->speed)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  //Set bits per word
  print_debug("Setting Xmega1 bits per word %d ... ",x1_setup->bits_per_word);
	if (ioctl(x1->fd, SPI_IOC_WR_BITS_PER_WORD, &(x1_setup->bits_per_word)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  print_debug("Setting Xmega2 bits per word %d ... ",x2_setup->bits_per_word);
	if (ioctl(x2->fd, SPI_IOC_WR_BITS_PER_WORD, &(x2_setup->bits_per_word)) < 0) {
    print_debug("FAIL\n");
		return 1;
	}
  print_debug("Success\n");

  return 0;
}

void Spi::setup_offset(Xmega_Data * xd, Xmega_Setup * xs){

  assert(xs->bytes_per_item[0] <= 4); //Assert bytes_per_index is at most 
  xd->num_bytes = xs->bytes_per_item[0];
  xd->offset_lookup[0] = 0;
  for(int i=1; i<xs->num_items; i++){
    assert(xs->bytes_per_item[i] <= 4); //Assert bytes_per_index is at most 4
    xd->num_bytes += xs->bytes_per_item[i];
    xd->offset_lookup[i] = xd->offset_lookup[i-1] + xs->bytes_per_item[i-1];
  }

}
