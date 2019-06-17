#include "Configurator.h"
#include <string>

Configurator ConfiguratorManager::config;

bool Configurator::openConfigFile(const string& fileName, bool is_flight_plan) {
  inFile.open(fileName);
  if (!inFile) {
    return false;
  }

  // If Loading a flight plan, only allow ONE flight plan at a time.
  if (is_flight_plan) {
    flightPlan.clear();
  }

  loadValues(is_flight_plan);

  inFile.close();
  return true;
}

void Configurator::loadValues(bool is_flight_plan) {
  string varName;
  string val;

  // Parse the file. Uses nice C++ syntax. 
  while (inFile >> varName) {
    inFile >> val;

    if (is_flight_plan) {
      // In case of flight plan, add times to flightPlan array
      flightPlan.push_back(std::make_pair(std::stoll(varName), (int16_t)std::stoi(val)));
    } else {
      // Normal case, just add pair to map
      mapVals.insert(pair<string, string> (varName, val));
    }

    // This ignores characters at the end of the line. allows for comments!
    inFile.ignore(200, '\n');    
  }
}

void Configurator::clear() {
  flightPlan.clear();
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

int16_t Configurator::getFlightPlan(int64_t time, int * start_index) {
  int16_t candidate = 0;
  // Iterate over the flight plan to find the appropriate motor setting
  for (int i = *start_index; i < flightPlan.size(); i++) {
    pair<int64_t, int16_t> & el = flightPlan[i];
    if (el.first <= time) {
      // We are greater than or equal to this time index
      // This is a possible motor speed
      candidate = el.second;
      // This is a slight hack, to improve performance. 
      // dont have to iterate over the entire list next time
      *start_index = i;  
    } else {
      // We found the highest acceptable time
      break;
    }
  }
  return candidate;
}

