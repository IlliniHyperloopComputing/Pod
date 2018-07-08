#ifndef MOTORMANAGER_HPP
#define MOTORMANAGER_HPP

#include "Utils.h"

namespace MotorManager {

  /*
   * Arms the motors
   */
  void enable_motors();

  /*
   * Disarms the motors
   */
  void disable_motors();

  /*
   * Sets the motor throttle to a specific value
   */
  void set_throttle(uint8_t value);

}

#endif
