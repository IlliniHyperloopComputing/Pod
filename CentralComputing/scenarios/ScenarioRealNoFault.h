#ifndef SCENARIOREALNOFAULT_H_
#define SCENARIOREALNOFAULT_H_
#include "Scenario.hpp"

class ScenarioRealNoFault : public Scenario {
 public:
  ScenarioRealNoFault();

  bool use_motion_model();
  std::shared_ptr<ADCData> sim_get_adc();
  std::shared_ptr<CANData> sim_get_can();
  std::shared_ptr<I2CData> sim_get_i2c();
  std::shared_ptr<PRUData> sim_get_pru();
  int64_t pru_delta_seconds, can_delta_seconds;
};
#endif