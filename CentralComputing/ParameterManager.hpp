#ifndef PARAMETER_MANAGER_H
#define PARAMETER_MANAGER_H

#include "Temperature.hpp"
#include "NetworkManager.hpp"

namespace ParameterManager {

extern Temperature temperature; 

vector<uint8_t> get_network_packet();

}



#endif
