#ifndef CAN_MANAGER_HPP
#define CAN_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct CANData {
  //replace with actual data structure
  int dummy_data;
};

class CANManager : public SourceManagerBase<CANData, false> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<CANData> refresh();
    std::shared_ptr<CANData> refresh_sim();
    long long refresh_timeout();

    std::string name(){
      return "can";
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif
