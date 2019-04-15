#include "Scenario.hpp"

#define MAX_ACCEL 9.81
#define MAX_DECEL -9.81

class ScenarioBasic : public Scenario {
 public:

  bool use_motion_model();
  std::shared_ptr<MotionData> sim_get_motion();
};