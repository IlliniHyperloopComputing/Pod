#ifndef PARAMETER_MANAGER_H
#define PARAMETER_MANAGER_H

#include "Velocity.hpp"
#include "Temperature.hpp"
#include "NetworkManager.hpp"


namespace ParameterManager {

extern Velocity velocity; 
extern Temperature temperature; 

vector<uint8_t> get_network_packet();

}



#endif
