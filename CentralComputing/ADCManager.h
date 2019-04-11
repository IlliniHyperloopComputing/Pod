#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include "SourceManagerBase.hpp"
#include "Defines.hpp"
#include <fstream>
using std::ifstream;

struct RawADCData {
    double raw_accel[NUM_ACCEL];
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

  std::string fileName;
  ifstream inFile;
};

#endif  // ADCMANAGER_H_
