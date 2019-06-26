#ifndef SCENARIOSENSORFREE_H_
#define SCENARIOSENSORFREE_H_
#include "Scenario.hpp"

class ScenarioBasic : public Scenario {
 public:

  bool use_sensor_free_motion();
  std::shared_ptr<MotionData> sim_get_motion();

};
#endif