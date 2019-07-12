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
  void initialize_sensor_error_configs();
  void check_for_sensor_error(const std::shared_ptr<I2CData> &, E_States state);

  int32_t error_general_1_over_temp;
  int32_t error_general_2_over_temp;
  int32_t error_general_3_over_temp;
  std::shared_ptr<I2CData> old_data; 

  std::string name() {
    return "i2c";
  }

  int i = 0;
  int j = 0;
  int i2c_fd = 0;

  unsigned char buffer[NUM_TMP];
};

#endif  // I2CMANAGER_H_
