#include "Configurator.h"
#include <string>

Configurator ConfiguratorManager::config;

bool Configurator::openConfigFile(const string& fileName, bool is_flight_plan) {
  inFile.open(fileName);
  if (!inFile) {
    return false;
  }
  loadValues(is_flight_plan);
  inFile.close();
  return true;
}

void Configurator::loadValues(bool is_flight_plan) {
  string varName;
  string val;
  while (inFile >> varName) {
    inFile >> val;
    if (is_flight_plan) {
      flightPlan.push_back(std::make_pair<int64_t, int64_t>(std::stoll(varName), std::stoll(val)));
    }
    else {
      // Normal case, just add pair to map
      mapVals.insert(pair<string, string> (varName, val));
    }
    inFile.ignore(200, '\n');  // This ignores characters at the end of the line. allows for comments
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

