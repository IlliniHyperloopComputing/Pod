#ifndef ADC_MANAGER_HPP
#define ADC_MANAGER_HPP

#include "SourceManagerBase.hpp"
#include "SourceManager.hpp"
#include <fstream>
using std::ifstream;

#define NUM_INPUTS 7 

struct ADCData {
  double accel[NUM_INPUTS];
};
struct RawADCData {
    double raw_accel[NUM_INPUTS];
};

class ADCManager : public SourceManagerBase<(long long) (1.0 * 1E6), ADCData, true> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<ADCData> refresh();
    std::shared_ptr<ADCData> refresh_sim();

    std::string name(){
      return "adc";
    }

    std::string fileName;
    ifstream inFile;
};

#endif

