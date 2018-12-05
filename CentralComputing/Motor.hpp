#ifndef MOTOR_HPP
#define MOTOR_HPP

#include "Utils.h"
#include "SourceManager.hpp"
#include "Simulator.hpp"
#include "Filter.h"
#include <cmath>


#define KP 1.0
#define KD 0.0
#define KI 1.0

class Motor {
  public:

    /*
     * Constructor
     * Setup the PWM pins
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

    /*
     * Uses a PI loop to calculate the appropriate motor throttle
     */
    int16_t calculate_throttle(double dt, int16_t last_throttle);

    /*
     * Sets the motor throttle to a specific value
     */
    void set_throttle(int16_t value);

    void debug_set_pwm_pin(std::string & path);

  private:

    void set_enable(bool enable);

    static std::string pwm_chip;
    static std::string pwm_pin;

    double integral;

    bool enabled;

};

#endif
