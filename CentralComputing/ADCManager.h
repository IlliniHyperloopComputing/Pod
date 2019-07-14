#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include "SourceManagerBase.hpp"
#include "Defines.hpp"
#include "Command.h"
#include <fstream>
#include <stdlib.h>
using std::ifstream;

class ADCManager : public SourceManagerBase<ADCData> {
 public:
  void calculate_zero_g();

 private:
  bool initialize_source();
  void stop_source();
  std::shared_ptr<ADCData> refresh();
  std::shared_ptr<ADCData> refresh_sim();

  int64_t calculate_zero_g_timeout;  // Calculate the zero g for X ammount of seconds
  int64_t calculate_zero_g_time;  // variable used in timer
  int64_t zero_g_sum[2];  // used while calculating average
  int64_t zero_g_num_samples;  // used while calculating average
  int64_t num_samples_zero_g;
  bool do_calculate_zero_g;
  int32_t default_zero_g;


  int16_t accel1_zero_g;
  int16_t accel2_zero_g;

  int32_t error_accel_diff;
  int32_t error_pneumatic_1_over_pressure;
  int32_t error_pneumatic_2_over_pressure;
  int32_t error_pneumatic_3_over_pressure;
  int32_t error_pneumatic_4_over_pressure;
  int32_t error_battery_box_over_pressure;
  int32_t error_battery_box_under_pressure;
  int32_t accel_diff_counter_error;
  int32_t accel_diff_counter;
  std::string name() {
    return "adc";
  }

  std::string fileName;
  ifstream inFile;

 public:
  // Public for testing purposes
  void initialize_sensor_error_configs();
  void check_for_sensor_error(const std::shared_ptr<ADCData> &, E_States state);
};

#endif  // ADCMANAGER_H_
