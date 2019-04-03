#include "MotionModel.h"

using Utils::print;
using Utils::LogLevel;
using Utils::microseconds;

std::shared_ptr<MotionData> MotionModel::refresh() {
  // Grab current time
  int64_t cur_time = microseconds();
  // Calculate time delta
  //int64_t dt = cur_time - last_time;
  // Set last time
  //last_time = cur_time;

  // Grab data
  //std::shared_ptr<PRUData> pru = SourceManager::PRU.Get();
  //std::shared_ptr<ADCData> adc = SourceManager::ADC.Get();

  // Measured state variable
  MotionData meas;
  //meas.x[0] = MotionModel::Median(pru.get()->encoder_distance, NUM_ENC_INPUTS);
  //meas.x[1] = MotionModel::Median(pru.get()->encoder_velocity, NUM_ENC_INPUTS);
  //meas.x[2] = MotionModel::Median(adc.get()->accel, NUM_ACCEL); 
  //meas.rpm  = MotionModel::Median(pru.get()->disk_RPM, NUM_MOTOR_INPUTS);
  //meas.fM = MotionModel::motor_profile(meas.x[1], meas.rpm);
  //meas.fD = MotionModel::drag_profile(meas.x[1]);
  


  MotionData gain;
  gain.x[0] = 1;
  gain.x[1] = 1;
  gain.x[2] = 1;
  gain.rpm = 1;
  gain.fM = 1;
  gain.fD = 1;

  // Apply Constant Gain Filter
  // MotionModel::ConstantGainFilter(&state, meas, gain, dt);

  // Return state
  std::shared_ptr<MotionData> new_data = std::make_shared<MotionData>();
  //*new_data = state;

  return new_data;
}

// get MotionData object
std::shared_ptr<MotionData> MotionModel::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_motion();
  #else
  return std::shared_ptr<MotionData>();
  #endif
}

double MotionModel::LowPass(double t_old, double t_new) {
  return t_old * LOWPASS_PERCENT + t_new * (1.0 - LOWPASS_PERCENT);
}

void MotionModel::ConstantGainFilter(MotionData * state, 
                                const MotionData & meas, 
                                const MotionData & gain,
                                double delta) {
  // Propagate state forward
  MotionData estimate;
  estimate.x[0] = state->x[0] + state->x[1] * delta;  // propagate distance 
  estimate.x[1] = state->x[1] + state->x[2] * delta;  // propagate velocity
  
  // RPM should track velocity
  // Take velocity slope (acceleration) and propagate forward
  estimate.rpm = state->rpm + (state->x[2] * delta) * MS_TO_RPM;

  // Calculate estimated Acceleration
  estimate.fM = motor_profile(estimate.x[1], estimate.rpm);  // Motor force
  estimate.fD = drag_profile(estimate.x[1]);  // Drag force

  // Estimate acceleration based on input forces
  estimate.x[2] = (state->fM + state->fD) / MASS;


  // Calculate residual between measurment and estimate
  MotionData residual;
  residual.x[0] = meas.x[0] - estimate.x[0];
  residual.x[1] = meas.x[1] - estimate.x[1];
  residual.x[2] = meas.x[2] - estimate.x[2];
  residual.fM   = meas.fM   - estimate.fM;
  residual.fD   = meas.fD   - estimate.fD;
  residual.rpm  = meas.rpm  - estimate.rpm;


  // Calculate new state
  state->x[0] = estimate.x[0] + gain.x[0] * residual.x[0];
  state->x[1] = estimate.x[1] + gain.x[1] * residual.x[1];
  state->x[2] = estimate.x[2] + gain.x[2] * residual.x[2];
  state->fM   = estimate.fM   + gain.fM   * residual.fM;
  state->fD   = estimate.fD   + gain.fD   * residual.fD;
  state->rpm  = estimate.rpm  + gain.rpm  * residual.rpm;
}

double MotionModel::drag_profile(double velocity) {
  // Apply estimated drag from aero/ friction/ wheel sources
  // Should be based on velocity
  return 0;
}

double MotionModel::motor_profile(double velocity, double RPM) {
  // Based on profiling curve, calculate velocity.
  // Maybe we just have a lookup table?
  return 0;
}
