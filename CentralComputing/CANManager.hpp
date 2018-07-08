#ifndef CAN_MANAGER_HPP
#define CAN_MANAGER_HPP

#include "SourceManagerBase.hpp"

class CANManager : public SourceManagerBase {

  public:

    CANManager(double delay_in_seconds):
      SourceManagerBase(delay_in_seconds)
    {}

    int getCANData(){
      return 7;
    }

  private:

    void refresh(){
      //Do work here to update data
      //this is running in its own thread
    }
};

#endif

