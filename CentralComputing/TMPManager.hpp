#ifndef TEMPERATURE_MANAGER_HPP
#define TEMPERATURE_MANAGER_HPP

#include "SourceManagerBase.hpp"
#include <string>
#include <iostream>

#define NUM_TMP 4

struct TMPData {
  double tmp_data[NUM_TMP];
};

class TMPManager : public SourceManagerBase<(long long) (1.0 * 1E6), TMPData> {
  private:
    shared_ptr<TMPData> refresh() {

      //create the data struct that we will return
      shared_ptr<TMPData> new_data = make_shared<TMPData>();

      //open the file
      std::ifstream in(prefix + devices[idx] + suffix);

      //If valid input
      if (in){
        //File will have two lines. we want the second line
        std::string line;
        std::getline(in, line);//ignore this line
        std::getline(in, line);//we want this line

        //parse the temperature value
        int rawTmp = std::stoi(line.substr(line.find('t')+2));

        //Set the new value
        old_data.tmp_data[idx] = rawTmp/1000.0;
      }
      
      //Copy the values into the new_data struct
      memcpy(new_data.get(), &old_data, sizeof(TMPData));

      //increment the index
      idx = (idx+1) % NUM_TMP;

      return new_data;
    }

    std::string prefix = "/sys/bus/w1/devices/";
    std::string suffix = "/w1_slave";
    std::string devices[NUM_TMP] = {"28-0417028e39ff", 
                                    "28-0417029885ff", 
                                    "28-0417029a0dff", 
                                    "28-051701e603ff"};

    int idx = 0;

    TMPData old_data;

};

#endif
