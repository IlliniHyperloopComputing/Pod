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

  acceleration = 0;

  // Paremters are loaded using constructor defined in Scenario.hpp
  if (motorsOn) {
    // Motor throttle is in thousandths of the rated torque. Throttle ranges from 0 to 1000
    double emrax_torque = rated_torque * throttle / 1000.0;  // units are now Nm
    double wheel_torque = emrax_torque / gear_ratio; 
    double force_at_wheel_edge = wheel_torque = wheel_torque / drive_wheel_radius;

    acceleration += force_at_wheel_edge / mass;
  }

  if (brakesOn && velocity > 0) {
    acceleration += brake_deceleration;
  }
  else if (velocity < 0) {
    lastVelocity = 0;
    velocity = 0;
  }

  // Apply kinematics
  velocity = lastVelocity + (acceleration * deltaSeconds);
  position = lastPosition + ((lastVelocity + velocity)/2 * deltaSeconds) 
              + (0.5 * acceleration * deltaSeconds * deltaSeconds);
  lastPosition = position;
  lastVelocity = velocity;

  timeLast = Utils::microseconds();
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
    "TRUE Motion: Pos: %.2f, Vel: %.2f, Acl: %.2f \n", position, velocity, acceleration );

  std::shared_ptr<PRUData> d = std::make_shared<PRUData>();
  d->wheel_velocity[0] =  velocity * 1000; // multiply by 1000 to convert to millimeters
  d->wheel_velocity[1] =  velocity * 1000;

  d->orange_distance[0] =  std::floor( position * 1000 / rear_wheel_circumfrence) * rear_wheel_circumfrence; // multiply by 1000 to convert to millimeters
  d->orange_distance[1] = d->orange_distance[0];
  d->wheel_distance[0] =  std::floor( position * 1000 / dist_between_orange) * dist_between_orange; 
  d->wheel_distance[1] =  d->orange_distance[0];

  return d;
}

#endif