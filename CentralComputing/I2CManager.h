#ifndef I2CMANAGER_H_
#define I2CMANAGER_H_

#include "SourceManagerBase.hpp"
#include "Defines.hpp"
#include "Configurator.h"
#include <fcntl.h>
#include <linux/i2c-dev.h>  // I2C bus definitions

#define ANC0 0x4  // 0b100
#define ANC1 0x5  // 0b101
#define ANC2 0x6  // 0b110
#define ANC3 0x7  // 0b111

class I2CManager : public SourceManagerBase<I2CData> {
 private:
  bool initialize_source();
  void stop_source();
  std::shared_ptr<I2CData> refresh();
  std::shared_ptr<I2CData> refresh_sim();
  bool single_shot(int fd, int port, int16_t * value);
  bool open_i2c(int * fd);
  bool set_i2c_addr(int fd, int addr);

  int32_t error_general_1_over_temp;
  int32_t error_general_2_over_temp;
  int32_t error_general_3_over_temp;
  std::shared_ptr<I2CData> old_data; 

  bool dps310_setup;
  uint8_t dps310_coef_buf[18];
  // pressure coefs
  int32_t dps310_coef_c00; // is a 20 bit number
  int32_t dps310_coef_c10; // is a 20 bit number
  int16_t dps310_coef_c01; // is a 16 bit number
  int16_t dps310_coef_c11; // is a 16 bit number
  int16_t dps310_coef_c20; // is a 16 bit number
  int16_t dps310_coef_c21; // is a 16 bit number
  int16_t dps310_coef_c30; // is a 16 bit number

  // temperature coefs
  int16_t dps310_coef_c0; // is a 12 bit number
  int16_t dps310_coef_c1;  // is a 12 bit number

  // coef source:
  // Highest bit is used.
  // 0 = ASCI temperature, internal.   1 = External temperature
  uint8_t dps310_coef_source;

  bool dps310_read_coef(int fd, uint8_t * read_buf);
  bool dps310_write_config_register(int fd, uint8_t reg, uint8_t value);
  bool dps310_get_temp_and_pressure(int fd, int32_t *temp, int32_t * pressure);


  std::string name() {
    return "i2c";
  }

  int i = 0;
  int j = 0;
  int i2c_fd = 0;

  unsigned char buffer[NUM_TMP];

 public:
  // Public for testing purposes
  void initialize_sensor_error_configs();
  void check_for_sensor_error(const std::shared_ptr<I2CData> &, E_States state);
};

#endif  // I2CMANAGER_H_
