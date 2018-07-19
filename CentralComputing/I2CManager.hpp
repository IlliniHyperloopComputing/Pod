#ifndef I2C_MANAGER_HPP
#define I2C_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct I2CData {
  //replace with actual data structure
  int dummy_data;
};

class I2CManager : public SourceManagerBase<(long long) (1.0 * 1E6), I2CData> {
  private:
    shared_ptr<I2CData> refresh() {
      //this is where you would query the I2C and get new data
      
      shared_ptr<I2CData> new_data = make_shared<I2CData>();
      new_data->dummy_data = i;
      i++;
      return new_data;
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif

