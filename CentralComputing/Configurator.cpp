#include "Configurator.hpp"

using namespace std;

Configurator ConfiguratorManager::config;

int Configurator::openVarFile(string fileName) {
    inFile.open(fileName);
    if(!inFile) {
        return 0;
    }
    loadValues();
    return 1;
}

void Configurator::loadValues() {
    inFile.clear();
    inFile.seekg(0, ios::beg);
    string varName;
    double val;
    int count = 0;
    while(inFile >> varName){
	inFile >> val;
	cout << varName << " " << val << " " << count<< endl;
	count++;
	mapVals.insert(pair<string, double> (varName, val));
    }
}

double Configurator::getValue(string varName) {
    if (mapVals.find(varName) == mapVals.end()) {
	return 0.0;
    }
    return mapVals.find(varName)->second;
}

