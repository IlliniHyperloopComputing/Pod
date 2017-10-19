#ifndef BRAKE_H
#define BRAKE_H
#include "Xmega.h"


/**
* Controllor class for brakes
**/
class Brake {
  public:

    /**
    * Default constructor
    **/
    Brake(Xmega * xmega);


    /**
    * Enables brakes, does NOT actuate
    **/
    void enable_brakes();

    /**
    * Disables relays, turns off ability to brake
    **/
    void disable_brakes();

    /**
    * Sets brake percentage to a value
    * @param value a value from 0-1, 0 for no brakes 1 for maximum brakes
    **/
    void set_brake_value(double value);

  private:
    #ifdef SIM

    #else

    #endif
};

#endif //BRAKE_H
