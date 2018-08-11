#include "Filter.h"

double Filter::LowPass(double t_old, double t_new){
  return t_old * LOWPASS_PERCENT + t_new * (1.0 - LOWPASS_PERCENT);
}

void Filter::ConstantGainFilter(  StateSpace & state, 
                                  StateSpace & meas, 
                                  StateSpace & gain,
                                  double delta){
  
  // Propagate state forward
  StateSpace estimate;
  estimate.x[0] = state.x[0] + state.x[1] * delta; //propagate distance 
  estimate.x[1] = state.x[1] + state.x[2] * delta; //propagate velocity

  // RPM should track velocity
  // Take velocity slope (acceleration) and propagate forward
  estimate.rpm = state.rpm + (state.x[2] * delta) * MS_TO_RPM;

  // Calculate estimated Acceleration
  estimate.fM = motor_profile(estimate.x[1], estimate.rpm); // Motor force
  estimate.fD = drag_profile( estimate.x[1]); // Drag force

  // Estimate acceleration based on input forces
  estimate.x[2] = (state.fM + state.fD) / MASS;


  // Calculate residual between measurment and estimate
  StateSpace residual;
  residual.x[0] = meas.x[0] - estimate.x[0];
  residual.x[1] = meas.x[1] - estimate.x[1];
  residual.x[2] = meas.x[2] - estimate.x[2];
  residual.fM   = meas.fM   - estimate.fM;
  residual.fD   = meas.fD   - estimate.fD;
  residual.rpm  = meas.rpm  - estimate.rpm;


  // Calculate new state
  state.x[0] = estimate.x[0] + gain.x[0] * residual.x[0];
  state.x[1] = estimate.x[1] + gain.x[1] * residual.x[1];
  state.x[2] = estimate.x[2] + gain.x[2] * residual.x[2];
  state.fM   = estimate.fM   + gain.fM   * residual.fM;
  state.fD   = estimate.fD   + gain.fD   * residual.fD;
  state.rpm  = estimate.rpm  + gain.rpm  * residual.rpm;

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
