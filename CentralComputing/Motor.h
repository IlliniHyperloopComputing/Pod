#ifndef MOTOR_H
#define MOTOR_H
#define IDEAL_RPM 5000 // Placeholder

#include "Utils.h"
#include <iostream>
#include <fstream>

/**
* The motor class controls the motors
* Its public interface abstracts sending commands to the Xmega
**/
class Motor {
  public:
    /**
    * Default constructor
    **/
    Motor();


    /**
    * Enables relays and turns on motors
    **/
    void enable_motors();

    /**
    * Disables relays and turns off motors
    **/
    void disable_motors();

    /**
    * Sets the target rpm
    * @param rpm target
    **/
    void set_target_rpm(double rpm);

    /**
    * Sets throttle value
    * 0 is minimum throttle, 100 is maximum
    **/
    bool set_throttle(double value);

    bool set_duty(uint64_t period);

    bool set_period(uint64_t period);

  private:

    static const std::string runPath;
    static const std::string dutyPath;
    static const std::string periodPath;

    #ifdef SIM
      
    #else
      //target rpm is the 
      double target_rpm;  
    #endif
};

#endif //MOTOR_H
