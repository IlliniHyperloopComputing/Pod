#ifndef MOTION_MODEL_HPP
#define MOTION_MODEL_HPP

#include "SourceManagerBase.hpp"
#include "SourceManager.hpp"
#include "PRUManager.hpp"
#include "ADCManager.hpp"
#include "Filter.h"

using namespace Utils;

#define MASS 1.0

struct StateSpace {
  double x[3]; //Position/Velocity/Accel
  //x[0] = x
  //x[1] = x' first derivative
  //x[2] = x'' second derivative
  
  double rpm; //RPM of disks

  double fM; //Force Motor
  double fD; //Force Drag
};

class MotionModel : public SourceManagerBase<(long long) (0.005 * 1E6), StateSpace, false> {
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

