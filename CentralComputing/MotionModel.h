#ifndef MOTIONMODEL_H_
#define MOTIONMODEL_H_

#include "Utils.h"
#include "Defines.hpp"
#include "Simulator.h"
#include "Configurator.h"
#include <memory>

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
  float LowPass(float t_old, float t_new, float lowpass_percent);

  template<class T>
  T Median(T * input, unsigned size);                          

 private:
  // Weights used in position calculation
  float w1, w2, w3, w4, w5, w6 ;
};

#endif  // MOTIONMODEL_H_

