#ifndef I2CMANAGER_H_
#define I2CMANAGER_H_

#include "SourceManagerBase.hpp"
#include "Defines.hpp"

class I2CManager : public SourceManagerBase<I2CData> {
 private:
  bool initialize_source();
  void stop_source();
  std::shared_ptr<I2CData> refresh();
  std::shared_ptr<I2CData> refresh_sim();
  void initialize_sensor_error_configs();
  void check_for_sensor_error(const std::shared_ptr<I2CData> &);

  int32_t error_general_1_over_temp;
  int32_t error_general_2_over_temp;
  int32_t error_general_3_over_temp;

  std::string name() {
    return "i2c";
  }

  // iterator for testing purposes, remove
  int i = 5;
};

#endif  // I2CMANAGER_H_
