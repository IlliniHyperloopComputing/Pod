#ifndef SOURCE_MANAGER_HPP
#define SOURCE_MANAGER_HPP

#include "PRUManager.hpp"
#include "CANManager.hpp"

namespace SourceManager {
  extern PRUManager PRU;
  extern CANManager CAN;
}

PRUManager SourceManager::PRU;
CANManager SourceManager::CAN;

#endif
