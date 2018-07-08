#ifndef VELOCITY_H
#define VELOCITY_H

#include "PodParameter.hpp"

class Velocity : public PodParameter<double> {
  public:
    Param<double> Get() {
      // Pull most up to date data from SourceManager
      // PRUManager.GetRPM()
      Param<double> p; 
      p.Value = 4.0;
      p.Timestamp = 9.0;
      return p;
    }
};

#endif
