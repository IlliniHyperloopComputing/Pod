#ifndef I2C_MANAGER_HPP
#define I2C_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct I2CData {
  //replace with actual data structure
  int dummy_data;
};

class I2CManager : public SourceManagerBase<(long long) (1.0 * 1E6), I2CData, false> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<I2CData> refresh();
    std::shared_ptr<I2CData> refresh_sim();

    std::string name(){
      return "i2c";
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif

