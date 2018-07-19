#ifndef TEMPERATURE_H 
#define TEMPERATURE_H 

#include "PodParameter.hpp"

class Temperature : public PodParameter<TMPData> {
  public:
    shared_ptr<TMPData> Get() {
      // Pull most up to date data from SourceManager
      shared_ptr<TMPData> p = make_shared<TMPData>(); 
      //replace with actual variables from PRUManager/OpticalManager
      p = SourceManager::TMP.Get();
      return p;
    }
};

#endif
