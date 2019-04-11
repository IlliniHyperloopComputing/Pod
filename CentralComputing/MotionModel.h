#ifndef MOTIONMODEL_H_
#define MOTIONMODEL_H_

#include "Utils.h"
#include "Defines.hpp"
#include "Simulator.h"
#include <memory>

#define LOWPASS_PERCENT 0.9

namespace MotionModel {
  // Source Manager methods
  void stop_source();
  void calculate(std::shared_ptr<UnifiedState> state);
  void calculate_sim(std::shared_ptr<UnifiedState> state);

  double LowPass(double t_old, double t_new);

  template<class T>
  T Median(T * input, unsigned size);                          

}  // namespace MotionModel

#endif  // MOTIONMODEL_H_

