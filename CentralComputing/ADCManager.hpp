#ifndef ADC_MANAGER_HPP
#define ADC_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct ADCData {
  //replace with actual data structure
  int dummy_data;
};

class ADCManager : public SourceManagerBase<(long long) (1.0 * 1E6), ADCData> {
  private:
    shared_ptr<ADCData> refresh() {
      //this is where you would query the ADC and get new data
      
      shared_ptr<ADCData> new_data = make_shared<ADCData>();
      new_data->dummy_data = i;
      i++;
      return new_data;
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif

