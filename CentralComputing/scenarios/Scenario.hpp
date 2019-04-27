#ifndef SCENARIO_H_
#define SCENARIO_H_
#include "Defines.hpp"
#include "Utils.h"

using Utils::print;
using Utils::microseconds;
using Utils::LogLevel;

// Abstract class 
class Scenario{
 public:
  
  // User must extend this class to implement this function
  virtual bool use_motion_model() = 0;

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

  void sim_motor_enable() {
    motorsOn = true;
    motors_on_time = microseconds();
  }
  
  void sim_motor_disable() {
    motorsOn = false;
  }
  
  void sim_motor_set_throttle(uint8_t value) {
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
  int64_t timeLast = -1;
  int64_t timeDelta = 0.000;
  bool motorsOn = false;
  bool brakesOn = false;
  uint8_t throttle = 0.000;
  uint8_t pressure = 0.000;
  double position = 0.000;
  double lastPosition = 0.000;
  double velocity = 0.000;
  double lastVelocity = 0.000;
  double acceleration = 0.000;
  int64_t motors_on_time = 0;
  int64_t brakes_on_time= 0;

};
#endif 