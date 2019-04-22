#ifdef SIM
#include "ScenarioRealLong.h"
#include "Utils.h"
using Utils::microseconds;
using Utils::clamp;

ScenarioRealLong::ScenarioRealLong() {
  pru_delta_seconds = microseconds();
  can_delta_seconds = pru_delta_seconds;
}

void ScenarioRealLong::true_motion() {
  timeDelta = Utils::microseconds() - timeLast;
  double deltaSeconds = static_cast<double>(timeDelta) / 1000000.0;

  if (motorsOn) {
    acceleration = 9 - 2.5 * clamp((microseconds() - motors_on_time)/10000000.0, 0.0, 1.0);
  } else if (brakesOn) {
    acceleration = -20;
  } else {
    acceleration = 0;
  }

  velocity = lastVelocity + (acceleration * deltaSeconds);
  position = lastPosition + ((lastVelocity + velocity)/2 * deltaSeconds) 
              + (0.5 * acceleration * deltaSeconds * deltaSeconds);
  lastPosition = position;
  lastVelocity = velocity;

  timeLast = Utils::microseconds();
}

bool ScenarioRealLong::use_motion_model() {
  return false;
}

std::shared_ptr<ADCData> ScenarioRealLong::sim_get_adc() {
  true_motion();
  std::shared_ptr<ADCData> d = std::make_shared<ADCData>();
  d->accel[0] =  acceleration * 1000; // multiply by 1000 to convert to millimeters
  d->accel[1] =  acceleration * 1000;
  d->accel[2] =  acceleration * 1000;
  return d;
}

std::shared_ptr<CANData> ScenarioRealLong::sim_get_can() {
  true_motion();
  std::shared_ptr<CANData> d = std::make_shared<CANData>();
  memset(d.get(), (uint8_t)0, sizeof(CANData));
  return d;
}

std::shared_ptr<I2CData> ScenarioRealLong::sim_get_i2c() {
  true_motion();
  std::shared_ptr<I2CData> d = std::make_shared<I2CData>();
  memset(d.get(), (uint8_t)0, sizeof(I2CData));
  return d;
}

std::shared_ptr<PRUData> ScenarioRealLong::sim_get_pru() {
  true_motion();
  print(LogLevel::LOG_DEBUG, 
    "Motion: Pos: %.2f, Vel: %.2f, Acl: %.2f \n", position, velocity, acceleration );

  std::shared_ptr<PRUData> d = std::make_shared<PRUData>();
  d->wheel_velocity[0] =  velocity * 1000; // multiply by 1000 to convert to millimeters
  d->wheel_velocity[1] =  velocity * 1000;

  d->orange_distance[0] =  position * 1000; // multiply by 1000 to convert to millimeters
  d->orange_distance[1] = d->orange_distance[0];
  d->wheel_distance[0] =  d->orange_distance[0];
  d->wheel_distance[1] =  d->orange_distance[0];

  return d;
}

#endif