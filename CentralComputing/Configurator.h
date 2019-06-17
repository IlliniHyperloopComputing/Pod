#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <vector>

using std::string;
using std::ifstream;
using std::map;
using std::vector;
using std::pair;

class Configurator {
 public:
  bool openConfigFile(const string&, bool is_flight_plan);
  void clear();
  bool getValue(const string&, string&);
  bool getValue(const string&, int64_t&);
  bool getValue(const string&, int32_t&);
  bool getValue(const string&, float&); 
  bool getValue(const string&, double&); 
  int16_t getFlightPlan(int64_t time, unsigned int * start_index);

 private:
  void loadValues(bool is_flight_plan);
  ifstream inFile;
  map <string, string> mapVals;
  vector <pair<int64_t, int16_t>> flightPlan;
};

namespace ConfiguratorManager {
  extern Configurator config;
}  // namespace ConfiguratorManager

#endif  // CONFIGURATOR_H_
