#ifndef VELOCITY_H
#define VELOCITY_H

#include "PodParameter.hpp"

class Velocity : public PodParameter<double> {
  public:
    shared_ptr<double> Get() {
      // Pull most up to date data from SourceManager
      shared_ptr<double> p = make_shared<double>(); 
      //replace with actual variables from PRUManager/OpticalManager
      *p = SourceManager::PRU.Get()->dummy_data;
      return p;
    }
};

#endif
