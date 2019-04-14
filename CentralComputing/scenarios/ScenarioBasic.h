#include "Scenario.hpp"

class ScenarioBasic : public Scenario {
 public:

  bool use_motion_model();
  std::shared_ptr<MotionData> sim_get_motion();
};