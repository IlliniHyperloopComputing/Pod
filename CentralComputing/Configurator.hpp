#include <fstream>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class Configurator {
    public:
	Configurator(string);

	void loadValues();

	double getValue(string);

	map <string, double> mapVals;

    private:
	ifstream inFile;
};

namespace ConfiguratorManager {
    extern Configurator* config;
}

int main() {
    //Configurator *test = new Configurator("test.txt");
    ConfiguratorManager::config = new Configurator("test.txt");
    ConfiguratorManager::config->loadValues();
    cout << "Variable is " << ConfiguratorManager::config->getValue("Variable") << endl;
    cout << "Number is " << ConfiguratorManager::config->getValue("Number") << endl;
}


