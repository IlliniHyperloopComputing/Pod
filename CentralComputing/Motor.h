#ifndef MOTOR_H_
#define MOTOR_H_

#include "Utils.h"
#include "SourceManager.h"
#include "Simulator.h"
#include <cmath>

#define MOTOR_OFF 0

class Motor {
 public:
  /*
   * Constructor
   */
  Motor();

  /*
   * Arms the motors
   */
  void enable_motors();

  /*
   * Disarms the motors
   */
  void disable_motors();

  /*
   * is_enabled returns enabled variable
   */
  bool is_enabled();

  int16_t get_throttle();

  /*
   * Sets the motor throttle to a specific value
   */
  void set_throttle(int16_t value);

  
  void set_relay_state(HV_Relay_Select, HV_Relay_State);

 private:
  void set_motor_state(bool enable);

  bool enabled;
  std::mutex mutex;

  int16_t throttle;
};

#endif  // MOTOR_H_ 
