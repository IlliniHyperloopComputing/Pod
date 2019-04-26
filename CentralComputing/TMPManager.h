#ifndef TMPMANAGER_H_
#define TMPMANAGER_H_

#include "SourceManagerBase.hpp"
#include <string>
#include <iostream>
#include <fstream>

#define NUM_TMP 4

struct TMPData {
  double tmp_data[NUM_TMP];
};

class TMPManager : public SourceManagerBase<TMPData> {
 private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<TMPData> refresh();
    std::shared_ptr<TMPData> refresh_sim();
    void initialize_sensor_error_configs();

    std::string prefix = "/sys/bus/w1/devices/";
    std::string suffix = "/w1_slave";
    std::string devices[NUM_TMP] = {"28-0417028e39ff",
                                    "28-0417029885ff",
                                    "28-0417029a0dff",
                                    "28-051701e603ff"};

    std::string name() {
      return "tmp";
    }

    int idx = 0;

    TMPData old_data;

    void check_for_sensor_error(const std::shared_ptr<TMPData> &);
};

#endif  // TMPMANAGER_H_
