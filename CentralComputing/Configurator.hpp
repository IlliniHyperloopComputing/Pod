#include <fstream>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class Configurator {
  public:
    bool openConfigFile(const string&);

    void loadValues();

    bool getValue(const string&, double&);

    map <string, double> mapVals;

  private:
	  ifstream inFile;
};

namespace ConfiguratorManager {
  extern Configurator config;
}




