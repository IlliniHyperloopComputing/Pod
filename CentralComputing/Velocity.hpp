#ifndef VELOCITY_H
#define VELOCITY_H

#include "PodParameter.hpp"

class Velocity : public PodParameter<double> {
  public:
    Param<double> Get() {
      // Pull most up to date data from SourceManager
      Param<double> p; 
      print_info("PRU.data * : %p\n", SourceManager::PRU.Get());
      p.Value = SourceManager::PRU.Get()->dummy_data;
      p.Timestamp = 9.0;
      return p;
    }
};

#endif
