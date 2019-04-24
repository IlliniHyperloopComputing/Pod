#include "MotionModel.h"

using Utils::print;
using Utils::LogLevel;
using Utils::microseconds;
using Utils::clamp;

MotionModel::MotionModel() {
  if (!(ConfiguratorManager::config.getValue("low_pass_filter_velocity", lpfv) &&
      ConfiguratorManager::config.getValue("low_pass_filter_acceleration", lpfa) &&
      ConfiguratorManager::config.getValue("motor_distance_clamp", motor_distance_clamp))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: MOTION_MODEL Missing necessary configuration\n");
    exit(1);
  }
}

// Orange tape sensor: Counts orange strips every 100 feet (41 stips in total). Position and velocity
//        Could undershoot, not overshoot. Assume 
//        Assume no "overcounting" of orange tape. Assume it could miss
// Wheel encoder: Counts rotations of the back suspension wheel, uses an optical encoder. Pos. and Vel.
//        Could undershoot, not overshoot. 
//        Assume wheel does not slip, or slips very little. 
//        Assume no "overcounting" of rotations. Assume it could miss
// Emrax (motor): Rotations and RPM (velocity) of the Emrax motor. Slip possible.
//        Could overshoot, not undershoot, due to slip while powered. 
// 
// Trust Orange tape and Wheel Encoder distances the most. 
//        Motor can add a bounded ammount of extra distance
// We trust the wheel encoder velocity the most
void MotionModel::calculate(std::shared_ptr<UnifiedState> state) {
  // POSITION
  int32_t orange_dist = std::max(state->pru_data->orange_distance[0], state->pru_data->orange_distance[1]);
  int32_t wheel_dist = std::max(state->pru_data->wheel_distance[0], state->pru_data->wheel_distance[1]);
  int32_t motor_dist = state->can_data->wheel_distance;

  // Take our minimum distance to be the maximum orange and wheel dist
  int32_t dist = std::max(orange_dist, wheel_dist);
  // Will accept that the motor is outputing a higher value, up to a certain limit
  dist = clamp(motor_dist, dist, dist + motor_distance_clamp);

  // VELOCITY
  // TODO: Refine this further. Incorporate other sensor inputs??
  // int32_t orange_vel = (state->pru_data->orange_velocity[0] + state->pru_data->orange_velocity[1]) / 2;  // average
  int32_t wheel_vel = (state->pru_data->wheel_velocity[0] + state->pru_data->wheel_velocity[1]) / 2;    // average
  // int32_t motor_vel = state->can_data->drive_wheel_velocity;
  // This is using low_pass_filter as a weighted average. 
  // int32_t vel = low_pass_filter(wheel_vel, orange_vel, 0.90);
  // vel = low_pass_filter(vel, motor_vel, 0.90);
  int32_t vel = wheel_vel;

  // ACCELERATION
  // Simply take the median of the three inputs.
  int32_t accl = Median(state->adc_data.get()->accel, NUM_ACCEL); 

  // Set state
  state->motion_data->x[0] = dist;
  state->motion_data->x[1] = low_pass_filter(state->motion_data->x[1], vel, lpfv);    
  state->motion_data->x[2] = low_pass_filter(state->motion_data->x[2], accl, lpfa);
}

// get MotionData object
void MotionModel::calculate_sim(std::shared_ptr<UnifiedState> state) {
  #ifdef SIM
  state->motion_data = SimulatorManager::sim.sim_get_motion(this, state);
  #endif
}

float MotionModel::low_pass_filter(float t_old, float t_new, float lowpass_percent) {
  return t_old * lowpass_percent + t_new * (static_cast<float>(1.0) - lowpass_percent);
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
    std::nth_element(input, input+size/2, input + size);
    return input[size/2];
  }
}
