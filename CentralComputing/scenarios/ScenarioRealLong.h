#ifndef SCENARIOREALNOFAULT2_H_
#define SCENARIOREALNOFAULT2_H_
#include "Scenario.hpp"

class ScenarioRealLong : public Scenario {
 public:
  ScenarioRealLong();

  bool use_motion_model();
  std::shared_ptr<ADCData> sim_get_adc();
  std::shared_ptr<CANData> sim_get_can();
  std::shared_ptr<I2CData> sim_get_i2c();
  std::shared_ptr<PRUData> sim_get_pru();

  virtual void true_motion();
  int64_t pru_delta_seconds, can_delta_seconds;
};
#endif