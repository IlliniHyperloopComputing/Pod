#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include "SourceManagerBase.hpp"
#include "Defines.hpp"

struct RawADCData {
  int dummy_data;
};

class ADCManager : public SourceManagerBase<ADCData> {
 private:
  bool initialize_source();
  void stop_source();
  std::shared_ptr<ADCData> refresh();
  std::shared_ptr<ADCData> refresh_sim();
  void initialize_sensor_error_configs();
  void check_for_sensor_error(const std::shared_ptr<ADCData> &);

  int32_t error_accel_diff;
  int32_t error_pneumatic_1_over_pressure;
  int32_t error_pneumatic_2_over_pressure;
  int32_t error_pneumatic_3_over_pressure;
  int32_t error_pneumatic_4_over_pressure;
  int32_t error_battery_box_over_pressure;
  int32_t error_battery_box_under_pressure;

  std::string name() {
    return "adc";
  }

  // iterator for testing purposes, remove
  int i = 5;
};

#endif  // ADCMANAGER_H_
