#ifndef POD_H
#define POD_H

#include "NetworkManager.hpp"
#include "ParameterManager.h"
#include "Pod_State.h"
#include "SourceManagerBase.hpp"
#include "PRUManager.hpp"

namespace SourceManager {
  // Add derived classes here
  extern PRUManager PRU;

}

void logic_loop();



#endif
