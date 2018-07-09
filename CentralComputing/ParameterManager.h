#ifndef PARAMETER_MANAGER_H
#define PARAMETER_MANAGER_H

#include "Velocity.hpp"
#include "Temperature.hpp"

namespace ParameterManager {
  extern Velocity velocity; 
  extern Temperature temperature; 
}

Velocity ParameterManager::velocity;
Temperature ParameterManager::temperature;

#endif
