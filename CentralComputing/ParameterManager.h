#ifndef PARAMETER_MANAGER_H
#define PARAMETER_MANAGER_H

#include "Velocity.hpp"
#include "Temperature.hpp"
#include "NetworkManager.hpp"


namespace ParameterManager {
  extern Velocity velocity; 
  extern Temperature temperature; 

  vector<uint8_t> get_network_packet() {
    vector<uint8_t> bytes;
    //TODO append all managed types bytes, use an identifier for each
    //Is there a better way to do this? Probably, but I'm lazy
    bytes.push_back(NetworkManager::
    return bytes;
  }
}

Velocity ParameterManager::velocity;
Temperature ParameterManager::temperature;


#endif
