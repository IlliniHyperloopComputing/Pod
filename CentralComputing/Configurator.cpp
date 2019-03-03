#include "Configurator.hpp"
#include <string>

using namespace std;

Configurator ConfiguratorManager::config;

bool Configurator::openConfigFile(const string& fileName) {
  inFile.open(fileName);
  if(!inFile) {
    return false;
  }
  loadValues();
  inFile.close();
  return true;
}

void Configurator::loadValues() {
  string varName;
  string val = "";
  while(inFile >> varName){
    inFile >> val;
    mapVals.insert(pair<string, string> (varName, val));
  }
}

bool Configurator::getValue(const string& varName, string& value) {
  if (mapVals.find(varName) == mapVals.end()) {
    return false;
  }
  value = mapVals.find(varName)->second;
  return true;
}

