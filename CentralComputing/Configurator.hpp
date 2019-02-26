#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H 

#include <fstream>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class Configurator {
  public:
    bool openConfigFile(const string&);
    bool getValue(const string&, double&);

  private:
    void loadValues();
    ifstream inFile;
    map <string, double> mapVals;
};

namespace ConfiguratorManager {
  extern Configurator config;
}

#endif
