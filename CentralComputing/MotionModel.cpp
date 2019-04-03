#include "MotionModel.h"

using Utils::print;
using Utils::LogLevel;
using Utils::microseconds;

void MotionModel::calculate(std::shared_ptr<UnifiedState> state){
  // Measured state variable
  MotionData meas;
  meas.x[0] = Median(state->pru_data.get()->encoder_distance, NUM_ENC_INPUTS);
  meas.x[1] = Median(state->pru_data.get()->encoder_velocity, NUM_ENC_INPUTS);
  meas.x[2] = Median(state->adc_data.get()->accel, NUM_ACCEL); 
  
  // Return state
  std::shared_ptr<MotionData> new_data = std::make_shared<MotionData>();
}

// get MotionData object
void MotionModel::calculate_sim(std::shared_ptr<UnifiedState> state){
  #ifdef SIM
  state->motion_data = SimulatorManager::sim.sim_get_motion();
  #endif
}

double MotionModel::LowPass(double t_old, double t_new) {
  return t_old * LOWPASS_PERCENT + t_new * (1.0 - LOWPASS_PERCENT);
}

template<class T>
T MotionModel::Median(T * input, unsigned size) {
  // Is even. Need to find two indicies, 
  // and then take the mean
  if (size % 2 == 0) {
    std::nth_element(input, input+size/2, input + size);
    std::nth_element(input + size/2, input+size/2, input+size);
    T sum = input[size/2] + input[size/2+1];
    return sum/2;
  } else {
    std::nth_element(input, input+size/2+1, input + size);
    return input[size/2 + 1];
  }
}