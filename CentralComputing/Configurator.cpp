#include "Configurator.hpp"

using namespace std;

Configurator* ConfiguratorManager::config;

Configurator::Configurator(string fileName) {
    inFile.open(fileName);
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

