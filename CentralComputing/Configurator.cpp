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
    if (mapVals.find(varName) == mapVals.end()) {
	return 0.0;
    }
    return mapVals.find(varName)->second;
}
