#ifndef SCENARIOREALNOFAULT2_H_
#define SCENARIOREALNOFAULT2_H_
#include "Scenario.hpp"

class ScenarioRealLong : public Scenario {
 public:
  ScenarioRealLong();

  virtual std::shared_ptr<ADCData> sim_get_adc();
  virtual std::shared_ptr<CANData> sim_get_can();
  virtual std::shared_ptr<I2CData> sim_get_i2c();
  virtual std::shared_ptr<PRUData> sim_get_pru();

  virtual void true_motion();
  int64_t pru_delta_seconds, can_delta_seconds, rolling_counter;
  int32_t adc_x_axis_0, adc_x_axis_1, adc_dir_flip;
};
#endif
