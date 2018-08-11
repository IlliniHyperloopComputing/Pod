#include "ParameterManager.hpp"

Temperature ParameterManager::temperature;

vector<uint8_t> ParameterManager::get_network_packet() {
  vector<uint8_t> bytes;
  //TODO append all managed types bytes, use an identifier for each
  //Is there a better way to do this? Probably, but I'm lazy
  
  //This is how you send the packet information for a single parameter
  //bytes.push_back(NetworkManager::Network_Data_ID::VELOCITY);
  //auto v_bytes = velocity.get_network_packet();
  //bytes.insert(bytes.end(), v_bytes.begin(), v_bytes.end());
 
  return bytes;
}
