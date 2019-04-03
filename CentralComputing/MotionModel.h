#ifndef MOTIONMODEL_H_
#define MOTIONMODEL_H_

#include "PRUManager.h"
#include "ADCManager.h"
#include "Filter.h"
#include "Simulator.h"

struct StateSpace {
  double x[3];  // Position/Velocity/Accel
  // x[0] = x
  // x[1] = x' first derivative
  // x[2] = x'' second derivative
  
  double rpm;  // RPM of disks

  double fM;  // Force Motor
  double fD;  // Force Drag
};

namespace MotionModel {
  // Source Manager methods
  bool initialize_source();
  void stop_source();
  std::shared_ptr<StateSpace> refresh();
  std::shared_ptr<StateSpace> refresh_sim();

  // Store our state
  extern StateSpace state;
  extern int64_t last_time;
}  // namepsace MotionModel

#endif  // MOTIONMODEL_H_

