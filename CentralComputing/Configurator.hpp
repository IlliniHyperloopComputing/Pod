#include <fstream>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class Configurator {
    public:
	int openVarFile(string);

	void loadValues();

	double getValue(string);

	map <string, double> mapVals;

    private:
	ifstream inFile;
};

namespace ConfiguratorManager {
    extern Configurator config;
}




