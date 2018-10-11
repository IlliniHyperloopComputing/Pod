#ifndef TEMPERATURE_MANAGER_HPP
#define TEMPERATURE_MANAGER_HPP

#include "SourceManagerBase.hpp"
#include <string>
#include <iostream>
#include <fstream>

#define NUM_TMP 4

struct TMPData {
  double tmp_data[NUM_TMP];
};

class TMPManager : public SourceManagerBase<(long long) (1.0 * 1E6), TMPData, false> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<TMPData> refresh();
    std::shared_ptr<TMPData> refresh_sim();

    std::string prefix = "/sys/bus/w1/devices/";
    std::string suffix = "/w1_slave";
    std::string devices[NUM_TMP] = {"28-0417028e39ff", 
                                    "28-0417029885ff", 
                                    "28-0417029a0dff", 
                                    "28-051701e603ff"};

    std::string name(){
      return "tmp";
    }

    int idx = 0;

    TMPData old_data;

};

#endif
