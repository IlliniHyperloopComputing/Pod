#include "I2CManager.h"

bool I2CManager::open_i2c(int & fd) {
  if ((fd = open("/dev/i2c-2" , O_RDWR)) < 0) {
    print(LogLevel::LOG_ERROR, "I2C Manager setup failed. Failed to open bus: %s\n", strerror(errno));
    return false;
  }

  return true;
}

bool I2CManager::set_i2c_addr(int fd, int addr) {
  if (ioctl(fd, I2C_SLAVE, addr) < 0) {
    print(LogLevel::LOG_ERROR, "I2C Manager setup failed. Failed to ioctl: %s\n", strerror(errno));
    return false;
  }

  return true;
}

bool I2CManager::initialize_source() {
  #ifndef BBB
  print(LogLevel::LOG_DEBUG, "I2C Manger setup failed. Not on BBB\n");
  return false;
  #endif

  if (!open_i2c(i2c_fd)) {
    return false;
  }

  if (!set_i2c_addr(i2c_fd, 0x48)) {
    return false;
  }
  
  print(LogLevel::LOG_DEBUG, "I2C Manger setup successful\n");
  return true;
}

void I2CManager::stop_source() {
  close(i2c_fd);
  print(LogLevel::LOG_DEBUG, "I2C Manger stopped\n");
}

bool I2CManager::single_shot(int fd, int port, int16_t & value) {
  uint8_t writeBuf[3];

  // set config register and start conversion
  // config register is 1
  writeBuf[0] = 1;    

  // bit 15 1 bit for single shot
  // Bits 14-12 input selection
  // 100 ANC0; 101 ANC1; 110 ANC2; 111 ANC3
  // Bits 11-9 Amp gain. Default to 010 here 001 P19
  // Bit 8 Operational mode of the ADS1115.
  // 0 : Continuous conversion mode
  // 1 : Power-down single-shot mode (default)
  // Bits 7-5 data rate to 111 for 860SPS
  // Bits 4-0  comparator functions see spec sheet.
  writeBuf[1] = 0x83;  // 0b10000011; // bits 15-8
  writeBuf[1] = writeBuf[1] | port << 4;  // bits 15-8
  writeBuf[2] = 0xe3;  // 0b11100011; // bits 7-0 

  // begin conversion
  if (write(fd, writeBuf, 3) != 3) {
    print(LogLevel::LOG_ERROR, "I2C Write error. Write config reg. %s\n", strerror(errno));
    return false;
  }

  // wait for conversion complete
  // checking bit 15
  do {
    if (read(fd, writeBuf, 2) != 2) {
      print(LogLevel::LOG_ERROR, "I2C Read error. Read config reg. %s\n", strerror(errno));
      return false;
    }
  } while ((writeBuf[0] & 0x80) == 0);

  // read conversion register -- first write register pointer
  uint8_t readBuf[3];   // conversion register is 0
  readBuf[0] = 0;   // conversion register is 0
  if (write(fd, readBuf, 1) != 1) {
    print(LogLevel::LOG_ERROR, "I2C Write error. Write conversion register. %s\n", strerror(errno));
    return false;
  }
  
  // read 2 bytes
  if (read(fd, readBuf, 2) != 2) {
    print(LogLevel::LOG_ERROR, "I2C Read error. Read conversion error. %s\n", strerror(errno));
    return false;
  }

  // convert display results
  value = readBuf[0] << 8 | readBuf[1];

  return true;
}

std::shared_ptr<I2CData> I2CManager::refresh() {
  std::shared_ptr<I2CData> new_data = std::make_shared<I2CData>();
  new_data->dummy_data = i;
  i++;

  int64_t a = Utils::microseconds(); 
  int16_t value = 0;
  if (!set_i2c_addr(i2c_fd, 0x48)) {
    return false;
  }
  if (single_shot(i2c_fd, ANC0, value)) {
    int64_t b = Utils::microseconds(); 
    print(LogLevel::LOG_INFO, "i2c val1: %d, in micros: %lu\n", value, b-a);
  }

  if (!set_i2c_addr(i2c_fd, 0x49)) {
    return false;
  }
  if (single_shot(i2c_fd, ANC1, value)) {
    print(LogLevel::LOG_INFO, "i2c val2: %d\n", value);
  }

  // myfloat = val * VPS; // convert to voltage
  return new_data;
}

std::shared_ptr<I2CData> I2CManager::refresh_sim() {
  return empty_data();
}
