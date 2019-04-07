#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <fstream>
#include <string>
#include <iostream>
#include <map>

using std::string;
using std::ifstream;
using std::map;
using std::pair;

class Configurator {
 public:
  bool openConfigFile(const string&);
  bool getValue(const string&, string&);
  bool getValue(const string&, int64_t&);
  bool getValue(const string&, float&); 

 private:
  void loadValues();
  ifstream inFile;
  map <string, string> mapVals;
};

namespace ConfiguratorManager {
  extern Configurator config;
}  // namespace ConfiguratorManager

#endif  // CONFIGURATOR_H_
