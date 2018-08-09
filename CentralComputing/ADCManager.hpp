#ifndef ADC_MANAGER_HPP
#define ADC_MANAGER_HPP

#include "SourceManagerBase.hpp"

struct ADCData {
  //replace with actual data structure
  int dummy_data;
};

class ADCManager : public SourceManagerBase<(long long) (1.0 * 1E6), ADCData> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<ADCData> refresh();

    //iterator for testing purposes, remove
    int i = 5;
};

#endif

