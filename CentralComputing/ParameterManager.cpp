#include "ParameterManager.hpp"


Velocity ParameterManager::velocity;
Temperature ParameterManager::temperature;

vector<uint8_t> ParameterManager::get_network_packet() {
  vector<uint8_t> bytes;
  //TODO append all managed types bytes, use an identifier for each
  //Is there a better way to do this? Probably, but I'm lazy
 
  return bytes;
}
