#include "MotionModel.h"

using Utils::print;
using Utils::LogLevel;
using Utils::microseconds;

MotionModel::MotionModel() {
  if((ConfiguratorManager::config.getValue("w1", w1) &&
      ConfiguratorManager::config.getValue("w2", w2) &&
      ConfiguratorManager::config.getValue("w3", w3) &&
      ConfiguratorManager::config.getValue("w4", w4) &&
      ConfiguratorManager::config.getValue("w5", w5) &&
      ConfiguratorManager::config.getValue("w6", w6))) {

  }
}

void MotionModel::calculate(std::shared_ptr<UnifiedState> state) {
  // The values w1, w2 and w3 will sum to 1 and will be tuned
  // w1 + w2 + w3 = 1, w4 + w5 = 1 and w5 + w6 = 1. All values will be tuned
  int32_t orange_dist = std::max(state->pru_data->orange_distance[0], state->pru_data->orange_distance[1]);
  int32_t wheel_dist = std::max(state->pru_data->wheel_distance[0], state->pru_data->wheel_distance[1]);
  int32_t motor_dist = state->can_data->wheel_distance;

  int32_t dist = orange_dist;
  if (wheel_dist > dist && motor_dist > dist) {
    dist = wheel_dist * w1 + dist * w2 + motor_dist * w3;
  } else if (wheel_dist > dist) {
    dist = wheel_dist * w4 + dist * w5;
  } else if (motor_dist > dist) {
    dist = motor_dist * w5 + dist * w6;
  } 

  // VELOCITY
  int32_t vels[3];
  vels[0] = std::max(state->pru_data->orange_velocity[0], state->pru_data->orange_velocity[1]);
  vels[1] = std::max(state->pru_data->wheel_velocity[0], state->pru_data->wheel_velocity[1]);
  vels[2] = state->can_data->drive_wheel_velocity;
  // TODO:  Take median? Take Max? Take average?
  int32_t vel = Median(vels, 3);

  // ACCELERATION
  int32_t accl = Median(state->adc_data.get()->accel, NUM_ACCEL); 

  // Set state
  state->motion_data->x[0] = dist;
  state->motion_data->x[1] = vel;   
  state->motion_data->x[2] = accl;
}

// get MotionData object
void MotionModel::calculate_sim(std::shared_ptr<UnifiedState> state) {
  #ifdef SIM
  state->motion_data = SimulatorManager::sim.sim_get_motion();
  #endif
}

float MotionModel::LowPass(float t_old, float t_new, float lowpass_percent) {
  return t_old * lowpass_percent + t_new * ((float)1.0 - lowpass_percent);
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
