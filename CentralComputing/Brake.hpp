#ifndef BRAKE_HPP
#define BRAKE_HPP

#include "Utils.h"
#include "SourceManager.hpp"
#include "MotionModel.hpp"
#include <cmath>

class Brake {
  public:

    /*
     * Constructor
     * Setup the PWM pins
     */
    Brake();

    /*
     * Arms the brakes
     */
    void enable_brakes();

    /*
     * Disarms the brakes
     */
    void disable_brakes();

    /*
     * is_enabled returns enabled variable
     */
    bool is_enabled();

    /*
     * Uses a <?> to calculate the appropriate brake force
     */
    int16_t calculate_brakeForce(double dt, int16_t last_brakeForce);

    /*
     * Sets the brake force to a specific value
     */
    void set_brakeForce(int16_t value);

    void debug_set_pwm_pin(std::string & path);

  private:

    void set_enable(bool enable);

    static std::string pwm_chip;
    static std::string pwm_pin;

    double integral;

    bool enabled;

};

#endif
