#include "I2CManager.hpp"

bool I2CManager::initialize_source(){

	
	// OPEN THE I2C BUS
	char *filename = (char*)"/dev/i2c-2";
	if ((i2c_fd = open(filename, O_RDWR)) < 0) {
    print(LogLevel::LOG_ERROR, "I2C Manager setup failed. Failed to open bus: %s\n", strerror(errno));
		return false;
	}

	//<<<<<The I2C address of the slave
	int addr = 0x48;	
  if (ioctl(i2c_fd, I2C_SLAVE, addr) < 0) {
    print(LogLevel::LOG_ERROR, "I2C Manager setup failed. Failed to ioctl: %s\n", strerror(errno));
		return false;
	}

  print(LogLevel::LOG_DEBUG, "I2C Manger setup successful\n");
  return true;
}

void I2CManager::stop_source(){

  close(i2c_fd);
  print(LogLevel::LOG_DEBUG, "I2C Manger stopped\n");
}

std::shared_ptr<I2CData> I2CManager::refresh() {
  std::shared_ptr<I2CData> new_data = std::make_shared<I2CData>();
  new_data->dummy_data = i;
  i++;

  uint8_t writeBuf[3];
	// set config register and start conversion
  // ANC1 and GND, 4.096v, 128s/s
  writeBuf[0] = 1;    // config register is 1

  writeBuf[1] = 0b11000011; // bits 15-8
  // bit 15 1 bit for single shot
  // Bits 14-12 input selection: 100
  // 100 ANC0; 101 ANC1; 110 ANC2; 111 ANC3
  // Bits 11-9 Amp gain. Default to 010 here 001 P19
  // Bit 8 Operational mode of the ADS1115.
  // 0 : Continuous conversion mode
  // 1 : Power-down single-shot mode (default)

  writeBuf[2] = 0b11100011; // bits 7-0 
  // Bits 7-5 data rate to 111 for 860SPS
  // Bits 4-0  comparator functions see spec sheet.

  // begin conversion
  if(write(i2c_fd, writeBuf, 3) != 3) {
    perror("Write to register 1");
    exit(-1);
  }

  // wait for conversion complete
  // checking bit 15
  do {
    if (read(i2c_fd, writeBuf, 2) != 2) {
      perror("Read conversion");
      exit(-1);
    }
  }
  while ((writeBuf[0] & 0x80) == 0);

	// read conversion register
  // write register pointer first
  readBuf[0] = 0;   // conversion register is 0
  if (write(fd, readBuf, 1) != 1) {
    perror("Write register select");
    exit(-1);
  }
  
  // read 2 bytes
  if (read(fd, readBuf, 2) != 2) {
    perror("Read conversion");
    exit(-1);
  }

  // convert display results
  val = readBuf[0] << 8 | readBuf[1];

  if (val < 0)   val = 0;

  myfloat = val * VPS; // convert to voltage

  printf("Values: HEX 0x%02x DEC %d reading %4.3f volts.\n",
         val, val, myfloat);

  return new_data;
}
std::shared_ptr<I2CData> I2CManager::refresh_sim() {
  return empty_data();
}
