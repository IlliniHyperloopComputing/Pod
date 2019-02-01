#include "Configurator.hpp"

using namespace std;

Configurator ConfiguratorManager::config;

void Configurator::openVarFile(string fileName) {
    inFile.open(fileName);
    loadValues();
}

void Configurator::loadValues() {
    inFile.clear();
    inFile.seekg(0, ios::beg);
    string varName;
    double val;
    while(inFile >> varName){
	inFile >> val;
	mapVals.insert(pair<string, double> (varName, val));
    }
}

double Configurator::getValue(string varName) {
    return mapVals.find(varName)->second;
}

int main() {
    ConfiguratorManager::config.openVarFile("test.txt");
    cout << "Variable is " << ConfiguratorManager::config.getValue("Variable") << endl;
}
