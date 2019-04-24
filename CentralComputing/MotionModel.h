#ifndef MOTIONMODEL_H_
#define MOTIONMODEL_H_

#include "Utils.h"
#include "Defines.hpp"
#include "Simulator.h"
#include "Configurator.h"
#include <memory>

// Class pre-defined in Defines.hpp, FYI
class MotionModel {
 public:
  // Used to read variables in from config file
  MotionModel();

  // Source Manager methods
  void calculate(std::shared_ptr<UnifiedState> state);
  void calculate_sim(std::shared_ptr<UnifiedState> state);

  // Basic digital lowpass filter. 
  // A higher lowpass_percent means you take less of the new value 
  // into account when calculating the output. 
  // EX: lowpass_percent = 1.00 means the output == t_old
  // EX: lowpass_percent = 0.00 means the output == t_new
  // Usually something like 0.90 (90%) is used, but it depends on how
  // frequently this function is called.
  float low_pass_filter(float t_old, float t_new, float lowpass_percent);

  template<class T>
  T Median(T * input, unsigned size);                          

 private:
  // Weights used in position calculation
  float lpfv, lpfa;
  int32_t motor_distance_clamp;  // used while calculating distance. See calculate()
};

#endif  // MOTIONMODEL_H_

