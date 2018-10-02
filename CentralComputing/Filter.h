#ifndef FILTER_H
#define FILTER_H

#include "MotionModel.hpp"
#include "Motor.hpp"
struct StateSpace; // stub for compiling

#define LOWPASS_PERCENT 0.9

namespace Filter{

  double LowPass(double t_old, double t_new);

  template<class T>
  T Median(T * input, unsigned size){
    // Is even. Need to find two indicies, 
    // and then take the mean
    if(size % 2 == 0){
      std::nth_element(input, input+size/2, input + size);
      std::nth_element(input + size/2, input+size/2, input+size);
      T sum = input[size/2] + input[size/2+1];
      return sum/2;
    }
    else{
      std::nth_element(input, input+size/2+1, input + size);
      return input[size/2 + 1];
    }

    
  }

  void ConstantGainFilter(StateSpace & state, 
                          StateSpace & meas, 
                          StateSpace & gain,
                          double delta);

  double drag_profile( double velocity);
  double motor_profile( double velocity, double RPM);

}

#endif
