#ifndef MOTORMANAGER_HPP
#define MOTORMANAGER_HPP

#include "Utils.h"
#include "SourceManager.hpp"
#include "MotionModel.hpp"
#include <cmath>

#define RPM_PDR 600.0  //point of diminishing returns
#define DISK_CIRCUMFRENCE 10.0
#define RPM_TO_MS (DISK_CIRCUMFRENCE/60.0)
#define MS_TO_RPM (60.0/DISK_CIRCUMFRENCE)
#define RPM_TO_USEC (25.0/400.0)

#define KP 1.0
#define KD 0.0
#define KI 1.0

class MotorManager {
  public:

    /*
     * Arms the motors
     */
    void enable_motors();

    /*
     * Disarms the motors
     */
    void disable_motors();

    /*
     * Uses a PI loop to calculate the appropriate motor throttle
     */
    uint16_t calculate_throttle(double dt, uint16_t last_throttle);

    /*
     * Sets the motor throttle to a specific value
     */
    void set_throttle(uint16_t value);

  private:
    double integral;


};

#endif
