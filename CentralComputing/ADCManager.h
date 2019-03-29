#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include "SourceManagerBase.hpp"
#include "SourceManager.h"

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

    std::string name() {
      return "adc";
    }

    // iterator for testing purposes, remove
    int i = 5;
};

#endif // ADCMANAGER_H_
