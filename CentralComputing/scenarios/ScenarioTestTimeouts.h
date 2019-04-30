#ifndef SCENARIOTESTTIMEOUTS_H_
#define SCENARIOTESTTIMEOUTS_H_
#include "ScenarioRealLong.h"

class ScenarioTestTimeouts: public ScenarioRealLong {
 public:
  ScenarioTestTimeouts();
  virtual void true_motion();
};
#endif