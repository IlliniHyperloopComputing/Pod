#include "ParameterManager.hpp"

Temperature ParameterManager::temperature;

vector<uint8_t> ParameterManager::get_network_packet() {
  vector<uint8_t> bytes;
  //TODO append all managed types bytes, use an identifier for each
  //Is there a better way to do this? Probably, but I'm lazy
  
  //This is how you send the packet information for a single parameter
  bytes.push_back(NetworkManager::Network_Data_ID::TEMPERATURE);
  auto t_bytes = temperature.get_network_packet();
  bytes.insert(bytes.end(), t_bytes.begin(), t_bytes.end());
 
  return bytes;
}
