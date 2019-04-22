#include "Configurator.h"
#include <string>

Configurator ConfiguratorManager::config;

bool Configurator::openConfigFile(const string& fileName) {
  inFile.open(fileName);
  if (!inFile) {
    return false;
  }
  loadValues();
  inFile.close();
  return true;
}

void Configurator::loadValues() {
  string varName;
  string val;
  while (inFile >> varName) {
    inFile >> val;
    mapVals.insert(pair<string, string> (varName, val));
    inFile.ignore(200, '\n');
  }
}

void Configurator::clear() {
  mapVals.clear();
}

bool Configurator::getValue(const string& varName, string& value) {
  if (mapVals.find(varName) == mapVals.end()) {
    return false;
  }
  value = mapVals.find(varName)->second;
  return true;
}

bool Configurator::getValue(const string& varName, int64_t& value) {
  if (mapVals.find(varName) == mapVals.end()) {
    return false;
  }
  value = std::stoll(mapVals.find(varName)->second);
  return true;
}

bool Configurator::getValue(const string& varName, int32_t& value) {
  if (mapVals.find(varName) == mapVals.end()) {
    return false;
  }
  value = std::stoll(mapVals.find(varName)->second);
  return true;
}

bool Configurator::getValue(const string& varName, float& value) {
  if (mapVals.find(varName) == mapVals.end()) {
    return false;
  }
  value = std::stod(mapVals.find(varName)->second);
  return true;
}

bool Configurator::getValue(const string& varName, double& value) {
  if (mapVals.find(varName) == mapVals.end()) {
    return false;
  }
  value = std::stod(mapVals.find(varName)->second);
  return true;
}

