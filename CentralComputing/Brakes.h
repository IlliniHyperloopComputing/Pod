#ifndef BRAKES_H_
#define BRAKES_H_

#include "Utils.h"
#include "Simulator.h"
#include <cmath>


class Brakes {
 public:
  /**
   * Constructor
   * Setup the PWM pins
   */
  Brakes();

  /**
   * Arms the motors
   */
  void enable_brakes();

  /**
   * Disarms the motors
   */
  void disable_brakes();

  /**
   * is_enabled returns enabled variable
   */
  bool is_enabled();

 private:
  void set_enable(bool enable);

  double integral;

  bool enabled;
};

#endif  // BRAKES_H_
