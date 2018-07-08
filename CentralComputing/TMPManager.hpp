#ifndef TEMPERATURE_MANAGER_HPP
#define TEMPERATURE_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct TMPData {
  //replace with actual data structure
  int dummy_data;
};

class TMPManager : public SourceManagerBase<(long long) (1.0 * 1E6), TMPData> {
  private:
    shared_ptr<TMPData> refresh() {
      //this is where you would query the PRU and get new data
      
      shared_ptr<TMPData> new_data = make_shared<TMPData>();
      new_data->dummy_data = i;
      i++;
      return new_data;
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif
