#ifndef CONFIGURATOR_H_
#define CONFIGURATOR_H_

#include <fstream>
#include <string>
#include <iostream>
#include <map>

using std::string;

class Configurator {
  public:
    bool openConfigFile(const string&);
    bool getValue(const string&, string&);
    bool getValue(const string&, long long&);
    bool getValue(const string&, double& ); 

  private:
    void loadValues();
    ifstream inFile;
    map <string, string> mapVals;
};

namespace ConfiguratorManager {
  extern Configurator config;
}

#endif // CONFIGURATOR_H_
