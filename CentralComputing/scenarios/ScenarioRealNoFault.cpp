#ifdef SIM
#include "ScenarioRealNoFault.h"
#include "Utils.h"
using Utils::microseconds;

#define MAX_ACCEL 9.81
#define MAX_DECEL -9.81

ScenarioRealNoFault::ScenarioRealNoFault(){
  pru_delta_seconds = microseconds();
  can_delta_seconds = pru_delta_seconds;
}

bool ScenarioRealNoFault::use_motion_model() {
  return false;
}

std::shared_ptr<ADCData> ScenarioRealNoFault::sim_get_adc() {
  std::shared_ptr<ADCData> d = std::make_shared<ADCData>();
  if (motorsOn) {
    d->accel[0] =  MAX_ACCEL * throttle;
    d->accel[1] =  MAX_ACCEL * throttle;
    d->accel[2] =  MAX_ACCEL * throttle;
    acceleration = MAX_ACCEL * throttle;
  } else if (brakesOn) {
    d->accel[0] =  MAX_ACCEL * pressure;
    d->accel[1] =  MAX_ACCEL * pressure;
    d->accel[2] =  MAX_ACCEL * pressure;
    acceleration = MAX_ACCEL * pressure;
  } else {
    d->accel[0] =  0;
    d->accel[1] =  0;
    d->accel[2] =  0;
    acceleration = 0;
  }

  // Multiply by 1000 to convert to millimeters
  d->accel[0] *= 1000;
  d->accel[1] *= 1000;
  d->accel[2] *= 1000; 
  return d;
}

std::shared_ptr<CANData> ScenarioRealNoFault::sim_get_can() {
  std::shared_ptr<CANData> d = std::make_shared<CANData>();
  memset(d.get(), (uint8_t)0, sizeof(CANData));
  return d;
}
std::shared_ptr<I2CData> ScenarioRealNoFault::sim_get_i2c() {
  std::shared_ptr<I2CData> d = std::make_shared<I2CData>();
  memset(d.get(), (uint8_t)0, sizeof(I2CData));
  return d;
}
std::shared_ptr<PRUData> ScenarioRealNoFault::sim_get_pru() {
  std::shared_ptr<PRUData> d = std::make_shared<PRUData>();
  int64_t td = microseconds() - pru_delta_seconds;

  double deltaSeconds = static_cast<double>(td) / 1000000.0;

  d->wheel_velocity[0] =  lastVelocity + (acceleration * deltaSeconds);
  d->wheel_velocity[1] =  d->wheel_velocity[0];
  velocity =  d->wheel_velocity[0];

  d->orange_distance[0] =  lastPosition + ((lastVelocity + velocity)/2 * deltaSeconds) 
              + (0.5 * acceleration * deltaSeconds * deltaSeconds);
  d->orange_distance[1] =  d->orange_distance[0];
  d->wheel_distance[0] =  d->orange_distance[0];
  d->wheel_distance[1] =  d->orange_distance[0];

  lastPosition = position;
  lastVelocity = velocity;
  pru_delta_seconds = microseconds();
  d->wheel_velocity[0] *= 1000; // multiply by 1000 to convert to millimeters
  d->wheel_velocity[1] *= 1000;
  d->orange_distance[0] *= 1000; 
  d->orange_distance[1] *= 1000;
  d->wheel_distance[0]  *= 1000; 
  d->wheel_distance[1]  *= 1000; 
  return d;
}

#endif