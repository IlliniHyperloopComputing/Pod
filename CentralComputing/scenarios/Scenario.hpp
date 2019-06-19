#ifndef SCENARIO_H_
#define SCENARIO_H_
#include "Defines.hpp"
#include "Utils.h"
#include "Configurator.h"

using Utils::print;
using Utils::microseconds;
using Utils::LogLevel;

// Base class 
class Scenario{
 public:

  // Use this to load in parameters from config file
  Scenario() {
    if (!(ConfiguratorManager::config.getValue("scn_gear_ratio", gear_ratio) && 
          ConfiguratorManager::config.getValue("scn_mass", mass) && 
          ConfiguratorManager::config.getValue("scn_brake_deceleration", brake_deceleration) && 
          ConfiguratorManager::config.getValue("scn_rated_torque", rated_torque) && 
          ConfiguratorManager::config.getValue("scn_rear_wheel_circ", rear_wheel_circumfrence) && 
          ConfiguratorManager::config.getValue("scn_dist_between_orange", dist_between_orange) && 
        ConfiguratorManager::config.getValue("scn_drive_wheel_radius", drive_wheel_radius))) {
      print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: (SCENARIO) Missing necessary configuration\n");
      exit(1);  // Crash hard on this error
    }
  }

  // If true, don't use the "sim_get_..." functions. 
  // This was derived from the most basic tests, before we used all of the
  // different sensor values to create our motion model.
  virtual bool use_sensor_free_motion() {
    return false;
  }

  // By default, all functions return "empty data"
  virtual std::shared_ptr<ADCData> sim_get_adc() {
    std::shared_ptr<ADCData> d = std::make_shared<ADCData>();
    memset(d.get(), (uint8_t)0, sizeof(ADCData));
    return d;
  }
  
  virtual std::shared_ptr<CANData> sim_get_can() {
    std::shared_ptr<CANData> d = std::make_shared<CANData>();
    memset(d.get(), (uint8_t)0, sizeof(CANData));
    return d;
  }

  virtual std::shared_ptr<I2CData> sim_get_i2c() {
    std::shared_ptr<I2CData> d = std::make_shared<I2CData>();
    memset(d.get(), (uint8_t)0, sizeof(I2CData));
    return d;
  }

  virtual std::shared_ptr<PRUData> sim_get_pru() {
    std::shared_ptr<PRUData> d = std::make_shared<PRUData>();
    memset(d.get(), (uint8_t)0, sizeof(PRUData));
    return d;
  }

  virtual std::shared_ptr<MotionData> sim_get_motion() {
    std::shared_ptr<MotionData> space = std::make_shared<MotionData>();
    space->x[0] = 0;
    space->x[1] = 0;
    space->x[2] = 0;
    return space;
  }

  void logging(bool enable) {
    enable_logging = enable;
  }

  void sim_relay_state(HV_Relay_Select relay, HV_Relay_State state) {
    ((char *)(&relay_state_buf))[relay] = state;
  }

  void sim_motor_state(bool enable) {
    if (enable) {
      motorsOn = true;
      motors_on_time = microseconds();
    }
    else {
      motorsOn = false;
    }
  }
  
  void sim_motor_set_throttle(int16_t value) {
    throttle = value;
  }
  
  void sim_brake_enable() {
    brakesOn = true;
    brakes_on_time = microseconds();
  }
  
  void sim_brake_disable() {
    brakesOn = false;
  }
  
  void sim_brake_set_pressure(uint8_t value) {
    pressure = value;
  }

  bool enable_logging = true;
  bool motorsOn = false;
  bool brakesOn = false;

  int64_t timeLast = -1;
  int64_t timeDelta = 0.000;
  uint32_t relay_state_buf = 0;

  int16_t throttle = 0;
  uint8_t pressure = 0;
  int64_t motors_on_time = 0;
  int64_t brakes_on_time= 0;

  double position = 0.000;
  double lastPosition = 0.000;
  double velocity = 0.000;
  double lastVelocity = 0.000;
  double acceleration = 0.000;


  // Config values:
  double gear_ratio = 0;
  double drive_wheel_radius = 0;
  double mass = 0;
  double brake_deceleration= 0;
  double rated_torque= 0;
  double rear_wheel_circumfrence = 0;
  double dist_between_orange = 0;
};
#endif 