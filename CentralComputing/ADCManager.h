#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include "SourceManagerBase.hpp"
#include "Defines.hpp"

struct RawADCData {
  int dummy_data;
};

class ADCManager : public SourceManagerBase<ADCData, true> {
 private:
  bool initialize_source();
  void stop_source();
  std::shared_ptr<ADCData> refresh();
  std::shared_ptr<ADCData> refresh_sim();
  void check_for_sensor_error(const std::shared_ptr<ADCData> &);

  std::string name() {
    return "adc";
  }

  // iterator for testing purposes, remove
  int i = 5;
};

#endif  // ADCMANAGER_H_
