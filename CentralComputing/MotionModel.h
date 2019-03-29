#ifndef MOTION_MODEL_HPP
#define MOTION_MODEL_HPP

#include "SourceManagerBase.hpp"
#include "SourceManager.h"
#include "PRUManager.h"
#include "ADCManager.h"
#include "StateSpace.hpp"
#include "Filter.h"

using namespace Utils;



class MotionModel : public SourceManagerBase<StateSpace, false> {
  public:

  private:
    // Source Manager methods
    bool initialize_source();
    void stop_source();
    std::shared_ptr<StateSpace> refresh();
    std::shared_ptr<StateSpace> refresh_sim();

    std::string name(){
      return "mm";
    }

    // Store our state
    StateSpace state;
    long long last_time;

};

#endif

