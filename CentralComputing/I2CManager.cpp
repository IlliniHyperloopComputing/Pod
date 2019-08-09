#include "I2CManager.h"

bool I2CManager::open_i2c(int * fd) {
  if ((*fd = open("/dev/i2c-2" , O_RDWR)) < 0) {
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
  dps310_setup = true;

  // Setup this variable which we will re-use
  old_data = std::make_shared<I2CData>();

  if (!open_i2c(&i2c_fd)) {
    set_error_flag(Command::Network_Command_ID::SET_I2C_ERROR, I2CErrors::I2C_SETUP_FAILURE);
    print(LogLevel::LOG_ERROR, "I2C Manger setup failed, can't open /dev/i2c\n");
    return false;
  }

  if (!set_i2c_addr(i2c_fd, 0x77)) {
    print(LogLevel::LOG_ERROR, "I2C Manger setup failed, can't set i2c address to read from\n");
    set_error_flag(Command::Network_Command_ID::SET_I2C_ERROR, I2CErrors::I2C_SETUP_FAILURE);
    return false;
  }

  // Setup dps310, read all of its coeficient registers (it has 10)
  if (!dps310_read_coef(i2c_fd, dps310_coef_buf)) {
    print(LogLevel::LOG_ERROR, "I2C DPS310 failed read coef, not exiting.\n");
    //set_error_flag(Command::Network_Command_ID::SET_I2C_ERROR, I2CErrors::I2C_SETUP_FAILURE);
    dps310_setup = false;
  }
  // write "sport" values, based on dps310 datasheet
  if (!( 
        // write operation register, turn off (move to standby mode)
        (dps310_write_config_register(i2c_fd, 0x08, 0x00)) &&
        // write pressure sensor, high frequency
        (dps310_write_config_register(i2c_fd, 0x06, 0x26)) &&
        // write temp sensor 64 measurements per second for temp register
        (dps310_write_config_register(i2c_fd, 0x07, 0x60 | dps310_coef_source)) &&
        // write cfg register, enable p-shift, disable everything else. NO FIFO
        (dps310_write_config_register(i2c_fd, 0x09, 0x04)) &&
        // write operation register, turn on (move to continous)
        (dps310_write_config_register(i2c_fd, 0x08, 0x07)))){
    print(LogLevel::LOG_ERROR, "I2C DPS310 failed write configs, not exiting.\n");
    //set_error_flag(Command::Network_Command_ID::SET_I2C_ERROR, I2CErrors::I2C_SETUP_FAILURE);
    dps310_setup = false;
  }
  
  print(LogLevel::LOG_DEBUG, "I2C Manger setup successful\n");
  return true;
}

void I2CManager::stop_source() {
  close(i2c_fd);
  print(LogLevel::LOG_DEBUG, "I2C Manger stopped\n");
}

bool I2CManager::single_shot(int fd, int port, int16_t *value) {
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
  *value = readBuf[0] << 8 | readBuf[1];

  return true;
}


bool I2CManager::dps310_read_coef(int fd, uint8_t * read_buf){
  uint8_t write_buf = 0x10;  // first register to read
  int pls = write(fd, &write_buf, 1);
  if (write(fd, &write_buf, 1) != 1) {  // Write to set the address that we will read from
    print(LogLevel::LOG_ERROR, "I2C Write error. DSP310 write coef addr %d\n",pls);
    return false;
  }
  if (read(fd, read_buf, 18) != 18) {  // read address 0x10 through 0x21
    print(LogLevel::LOG_ERROR, "I2C Read error. DSP310 read coefs\n");
    return false;
  }

  // parse temperature coefficients, convert to signed data
  dps310_coef_c0 = (read_buf[0] << 4) | ((read_buf[1] & 0xf0) >> 4);
  if(dps310_coef_c0 > 2048-1) {  
    dps310_coef_c0 -= 4096;  // minus 2^12
  }
  dps310_coef_c0 = ((read_buf[1] & 0x0f) << 8) | (read_buf[2]);
  if(dps310_coef_c0 > 2048-1) {  
    dps310_coef_c0 -= 4096;  // minus 2^12
  }

  // Since these are 16 bit values, casting into an int16 should be sufficient
  dps310_coef_c01 = (int16_t)((read_buf[8]  << 8) | (read_buf[9]));
  dps310_coef_c11 = (int16_t)((read_buf[10] << 8) | (read_buf[11]));
  dps310_coef_c20 = (int16_t)((read_buf[12] << 8) | (read_buf[13]));
  dps310_coef_c21 = (int16_t)((read_buf[14] << 8) | (read_buf[15]));
  dps310_coef_c30 = (int16_t)((read_buf[16] << 8) | (read_buf[17]));

  dps310_coef_c00 = ((read_buf[3] << 12) | (read_buf[4] << 4) | ((read_buf[5] & 0xf0) >> 4));
  if(dps310_coef_c00 > 524288-1) {    // 2^19 -1
    dps310_coef_c00 -= 1048576;  // minus 2^20
  }

  dps310_coef_c10 = (((read_buf[5] & 0x0f) << 16) | (read_buf[6] << 8) | (read_buf[7]));
  if(dps310_coef_c10 > 524288-1) {    // 2^19 -1
    dps310_coef_c10 -= 1048576;  // minus 2^20
  }

  write_buf = 0x28;  // first register to read
  if (write(fd, &write_buf, 1) != 1) {  // Write to set the address that we will read from
    print(LogLevel::LOG_ERROR, "I2C Write error. DSP310 write coef source addr \n");
    return false;
  }
  if (read(fd, read_buf, 1) != 1) {  // read address 0x10 through 0x21
    print(LogLevel::LOG_ERROR, "I2C Write error. DSP310 read coefs source\n");
    return false;
  }
  return true;
}

bool I2CManager::dps310_write_config_register(int fd, uint8_t reg, uint8_t value) {
  // write to reg with value
  uint8_t write_buf[2];
  write_buf[0] = reg;
  write_buf[1] = value;
  if (write(fd, &write_buf, 2) != 2) {  // Write to set the address that we will read from
    print(LogLevel::LOG_ERROR, "I2C Write error. DSP310 write config register error: reg: %x value: %x\n", reg, value);
    return false;
  }
  return true;
}

bool I2CManager::dps310_get_temp_and_pressure(int fd, int32_t *temp, int32_t * pressure) {

  //
  //
  //
  // Read Temperature First
  // Since we should be in continuous mode, just read from the registers!
  uint8_t write_buf = 0x03;  // first register to read
  if (write(fd, &write_buf, 1) != 1) {  // Write to set the address that we will read from
    print(LogLevel::LOG_ERROR, "I2C Write error. DSP310 write addr , while trying to get_temp\n");
    return false;
  }

  uint8_t read_buf[3];
  if (read(fd, read_buf, 3) != 3) {  // read address 0x03 through 0x05 
    print(LogLevel::LOG_ERROR, "I2C Read error. DSP310 read temp\n");
    return false;
  }
  int32_t temp_raw;
  temp_raw = (read_buf[0] << 16) | (read_buf[1] << 8) | (read_buf[2]);
  if(temp_raw > 8388608-1) {    // 2^23 -1
    temp_raw -= 16777216;  // minus 2^24
  }

  // temp Compensation scale factors  based on oversampling
  // selected no oversampling: 
  double kT = 524288;
  double t_raw_sc= ((double)temp_raw) / ((double) kT);
  *temp = (int32_t) ( ((double)dps310_coef_c0) * 0.5 + dps310_coef_c1 * t_raw_sc);

  //
  //
  //
  //
  //
  // Read pressure second
  // Since we should be in continuous mode, just read from the registers!
  write_buf = 0x00;  // first register to read
  if (write(fd, &write_buf, 1) != 1) {  // Write to set the address that we will read from
    print(LogLevel::LOG_ERROR, "I2C Write error. DSP310 write addr , while trying to get_temp_and_pressure\n");
    return false;
  }

  if (read(fd, read_buf, 3) != 3) {  // read address 0x00 through 0x02 
    print(LogLevel::LOG_ERROR, "I2C Read error. DSP310 read pressure\n");
    return false;
  }
  int32_t pressure_raw;
  pressure_raw = ((read_buf[0] << 16) | (read_buf[1] << 8) | (read_buf[2]));
  if (pressure_raw > 8388608-1) {    // 2^23 -1
    pressure_raw -= 16777216;  // minus 2^24
  }

  // Pressure Compensation scale factors  based on oversampling
  // oversamples 64 times (based on 0x26 config value)
  double kP = 1040384;
  double p_raw_sc = pressure_raw / kP;

  * pressure = (int32_t) (dps310_coef_c00 
                          + p_raw_sc * (dps310_coef_c10 + p_raw_sc * (dps310_coef_c20 + p_raw_sc * dps310_coef_c30))
                          + t_raw_sc * dps310_coef_c01 
                          + t_raw_sc * p_raw_sc * (dps310_coef_c11 + p_raw_sc * dps310_coef_c21));

  return true;
}

std::shared_ptr<I2CData> I2CManager::refresh() {
   
  if (i == 3) {
    i = 0;
  } else {
    if (j == 2) {
      j = 0;
    }
    i++;
  }
  

  j = 1;
  int addr = 0x49;
  int port;

  if (i == 0) {
    port = ANC0;
  }
  else if (i == 1) {
    port = ANC1;
  }
  //else if (i == 2) {
  //  port = ANC2;
  //}
  //else {
  //  port = ANC3;
  //}

  /* 
  if (j == 0) {
    addr = 0x48;
  }
  else if (j == 1) {
    addr = 0x49;
  }
  */

  int64_t a = Utils::microseconds();
  int16_t value = 0;

  if (!set_i2c_addr(i2c_fd, addr)) {
    Command::set_error_flag(Command::SET_I2C_ERROR, I2C_READ_ERROR);
    return empty_data();
  }
  if (single_shot(i2c_fd, port, &value)) {
    int64_t b = Utils::microseconds(); 
    // print(LogLevel::LOG_DEBUG, "(REMOVE) I2C: i=%d, j =%d, val: %d, took this long: micros: %lu\n", i, j, value, b-a);
  } else {
    Command::set_error_flag(Command::SET_I2C_ERROR, I2C_READ_ERROR);
    return empty_data();
  }
  int index = (j * 4) + i;

  if (dps310_setup) {
    if (!set_i2c_addr(i2c_fd, 0x77)) {
      print(LogLevel::LOG_DEBUG, "I2C DPS310 -- why is it failing now?");
    }
    dps310_get_temp_and_pressure(i2c_fd, &old_data->temp_sensor, &old_data->pressure_sensor);
  }

  // Update the "old" data with the new reading
  old_data->temp[index] = value;

  // duplicate the "old_data" here into the "new_data"
  std::shared_ptr<I2CData> new_data = std::make_shared<I2CData>(*old_data);

  // new_data contains both the new and old readings.
  return new_data;
}

std::shared_ptr<I2CData> I2CManager::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_i2c();
  #else
  return empty_data();
  #endif
}

void I2CManager::initialize_sensor_error_configs() {
  if (!(ConfiguratorManager::config.getValue("error_general_1_over_temp", error_general_1_over_temp) &&
      ConfiguratorManager::config.getValue("error_general_2_over_temp",   error_general_2_over_temp) &&
      ConfiguratorManager::config.getValue("error_general_3_over_temp",   error_general_3_over_temp))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: I2CManager Missing necessary configuration\n");
    exit(1);
  }
}

void I2CManager::check_for_sensor_error(const std::shared_ptr<I2CData> & check_data, E_States state) {
  // auto temp_arr = check_data->temp;
  // for (int index = 0; index < static_cast<int>(NUM_TMP / 3); index++) {
  //   if (temp_arr[index] > static_cast<int>(error_general_1_over_temp)) {
  //     Command::set_error_flag(Command::SET_I2C_ERROR, I2C_OVER_TEMP_ONE);
  //   }
  // }
  // for (int index = static_cast<int>(NUM_TMP / 3); index < static_cast<int>(2 * (NUM_TMP / 3)); index++) {
  //   if (temp_arr[index] > static_cast<int>(error_general_1_over_temp)) {
  //     Command::set_error_flag(Command::SET_I2C_ERROR, I2C_OVER_TEMP_TWO);
  //   }
  // }
  // for (int index = static_cast<int>(2 * (NUM_TMP / 3)); index < NUM_TMP; index++) {
  //   if (temp_arr[index] > static_cast<int>(error_general_1_over_temp)) {
  //     Command::set_error_flag(Command::SET_I2C_ERROR, I2C_OVER_TEMP_THREE);
  //   }
  // }
}
