#ifndef MOTOR_H
#define MOTOR_H
#include "Xmega.h"

/**
* The motor class controls the motors
* Its public interface abstracts sending commands to the Xmega
**/
class Motor {
  public:
    /**
    * Default constructor
    **/
    Motor(Xmega * xmega);


    /**
    * Enables relays and turns on motors
    **/
    void enable_motors();

    /**
    * Disables relays and turns off motors
    **/
    void disable_motors();

    /**
    * Sets the motor throttle to a value
    * @param value a value from 0-1, 0 for no throttle, 1 for maximum
    **/
    void set_throttle(double value);

  private:
    #if SIM

    #else

    #endif
};

#endif //MOTOR_H
