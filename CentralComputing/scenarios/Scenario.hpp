#include "Defines.hpp"
#include "Utils.h"
#include "SourceManager.h"

using Utils::print;
using Utils::LogLevel;

// Abstract class 
class Scenario{
 public:
  
  // User must extend this class to implement this function
  virtual bool use_motion_model() = 0;

  // By default, all functions return "empty data"
  std::shared_ptr<ADCData> sim_get_adc() {
    return SourceManager::ADC.empty_data();
  }
  std::shared_ptr<CANData> sim_get_can() {
    return SourceManager::CAN.empty_data();
  }
  std::shared_ptr<I2CData> sim_get_i2c() {
    return SourceManager::I2C.empty_data();
  }
  std::shared_ptr<PRUData> sim_get_pru() {
    return SourceManager::PRU.empty_data();
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
  }
  
  void sim_motor_disable() {
    motorsOn = false;
  }
  
  void sim_motor_set_throttle(uint8_t value) {
    throttle = value;
  }
  
  void sim_brake_enable() {
    brakesOn = true;
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

};