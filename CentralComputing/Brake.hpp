#ifndef Brake_HPP
#define Brake_HPP

#include "Utils.h"
#include "SourceManager.hpp"
#include "MotionModel.hpp"
#include <cmath>

//TODO: Better define these constants
#define RPM_PDR 600.0  //point of diminishing returns
#define DISK_CIRCUMFRENCE 10.0 //In Meters
#define RPM_TO_MS (DISK_CIRCUMFRENCE/60.0) // RPM to Meters per second
#define MS_TO_RPM (60.0/DISK_CIRCUMFRENCE) // Meters per second to RPM
//TODO: Better define this constant
#define RPM_TO_USEC (25.0/400.0) // Usec/RRPM

#define Brake_OFF -50.0


#define KP 1.0
#define KD 0.0
#define KI 1.0

class Brake {
  public:

    /*
     * Constructor
     * Setup the PWM pins
     */
    Brake();

    /*
     * Arms the Brakes
     */
    void enable_Brakes();

    /*
     * Disarms the Brakes
     */
    void disable_Brakes();

    /*
     * is_enabled returns enabled variable
     */
    bool is_enabled();

    /*
     * Uses a PI loop to calculate the appropriate Brake pressure
     */
    int16_t calculate_pressure(double dt, int16_t last_pressure);

    /*
     * Sets the Brake pressure to a specific value
     */
    void set_pressure(int16_t value);

    void debug_set_pwm_pin(std::string & path);

  private:

    void set_enable(bool enable);

    static std::string pwm_chip;
    static std::string pwm_pin;

    double integral;

    bool enabled;

};

#endif
