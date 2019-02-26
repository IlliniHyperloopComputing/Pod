#ifndef ADC_MANAGER_HPP
#define ADC_MANAGER_HPP

#include "SourceManagerBase.hpp"
#include "SourceManager.hpp"

#define NUM_ACCEL 3

struct ADCData {
  double accel[NUM_ACCEL];
  int dummy_data;
};
struct RawADCData {
  int dummy_data;
};

class ADCManager : public SourceManagerBase<ADCData, true> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<ADCData> refresh();
    std::shared_ptr<ADCData> refresh_sim();
    long long refresh_timeout();

    std::string name(){
      return "adc";
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif

