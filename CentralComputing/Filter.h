#ifndef FILTER_H
#define FILTER_H

#include "StateSpace.hpp"
#include <algorithm>

#define LOWPASS_PERCENT 0.9

//TODO: Better define these constants
#define RPM_PDR 600.0  //point of diminishing returns
#define DISK_CIRCUMFRENCE 10.0 //In Meters
#define RPM_TO_MS (DISK_CIRCUMFRENCE/60.0) // RPM to Meters per second
#define MS_TO_RPM (60.0/DISK_CIRCUMFRENCE) // Meters per second to RPM
//TODO: Better define this constant
#define RPM_TO_USEC (25.0/400.0) // Usec/RRPM
#define MASS 1.0

#define MOTOR_OFF -50.0

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
