#ifndef CAN_MANAGER_HPP
#define CAN_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct CANData {
  //replace with actual data structure
  int dummy_data;
};

class CANManager : public SourceManagerBase<(long long) (1.0 * 1E6), CANData> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<CANData> refresh();

    //iterator for testing purposes, remove
    int i = 5;
};

#endif
