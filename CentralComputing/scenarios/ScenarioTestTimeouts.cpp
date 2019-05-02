#ifdef SIM
#include "ScenarioTestTimeouts.h"
#include "Utils.h"
using Utils::microseconds;
using Utils::clamp;

ScenarioTestTimeouts::ScenarioTestTimeouts() {
}

void ScenarioTestTimeouts::true_motion() {
  timeDelta = Utils::microseconds() - timeLast;
  double deltaSeconds = static_cast<double>(timeDelta) / 1000000.0;

  if (motorsOn) {
    acceleration = 0; 
  } else if (brakesOn) {
    acceleration = 0;
    velocity = 0;
  } else {
    acceleration = 0;
  }

  timeLast = Utils::microseconds();
}


#endif