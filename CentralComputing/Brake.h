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
    Brake();


    /**
    * Enables brakes, does NOT actuate
    **/
    void enable_brakes();

    /**
    * Disables relays, turns off ability to brake
    **/
    void disable_brakes();


  private:
    #ifdef SIM

    #else

    #endif
};

#endif //BRAKE_H
