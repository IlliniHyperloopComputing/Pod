#ifndef CAN_MANAGER_HPP
#define CAN_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct CANData {
  //replace with actual data structure
  int dummy_data;
};

class CANManager : public SourceManagerBase<(long long) (1.0 * 1E6), CANData> {
  private:
    shared_ptr<CANData> refresh() {
      //this is where you would query the CAN and get new data
      
      shared_ptr<CANData> new_data = make_shared<CANData>();
      new_data->dummy_data = i;
      i++;
      return new_data;
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif
