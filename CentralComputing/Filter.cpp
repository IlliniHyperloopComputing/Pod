#include "Filter.h"

double Filter::LowPass(double t_old, double t_new){
  return t_old * LOWPASS_PERCENT + t_new * (1.0 - LOWPASS_PERCENT);
}

void Filter::ConstantGainFilter(  StateSpace & state, 
                                  StateSpace & meas, 
                                  StateSpace & gain,
                                  double mass,
                                  double delta){
  
  // Propagate state forward
  StateSpace estimate;
  estimate.x[0] = state.x[0] + state.x[1] * delta; //propagate distance 
  estimate.x[1] = state.x[1] + state.x[2] * delta; //propagate velocity

  // Calculate estimated Acceleration
  // State.fM and fD are based on == propagated state
  estimate.fM = motor_profile(estimate.x[1], state.rpm ); // WHICH RPM????
  estimate.fD = drag_profile( estimate.x[1]);

  estimate.x[2] = (state.fM + state.fD) / mass;


  // Calculate residual 
  StateSpace residual;
  residual.x[0] = meas.x[0] - estimate.x[0];
  residual.x[1] = meas.x[1] - estimate.x[1];
  residual.x[2] = meas.x[2] - estimate.x[2];
  residual.fM   = meas.fM   - estimate.fM;
  residual.fD   = meas.fD   - estimate.fD;


  // Calculate new state
  state.x[0] = estimate.x[0] + gain.x[0] * residual.x[0];
  state.x[1] = estimate.x[1] + gain.x[1] * residual.x[1];
  state.x[2] = estimate.x[2] + gain.x[2] * residual.x[2];
  state.fM   = estimate.fM   + gain.fM   * residual.fM;
  state.fD   = estimate.fD   + gain.fD   * residual.fD;
  state.rpm  = meas.rpm;

}

double Filter::drag_profile( double velocity){
  //Apply estimated drag from aero/ friction/ wheel sources
  //Should be based on velocity
  return 0;
}

double Filter::motor_profile( double velocity, double RPM){
  //Based on profiling curve, calculate velocity.
  //Maybe we just have a lookup table?
  return 0;
}
