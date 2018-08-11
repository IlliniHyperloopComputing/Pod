#ifndef MOTORMANAGER_HPP
#define MOTORMANAGER_HPP

#include "Utils.h"

#define RELATIVE_RPM_TARGET 600.0
#define DISK_CIRCUMFRENCE 10.0
#define RPM_TO_MS (DISK_CIRCUMFRENCE/60.0)
#define MS_TO_RPM (60.0/DISK_CIRCUMFRENCE)

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
