#ifndef SOURCE_MANAGER_HPP
#define SOURCE_MANAGER_HPP

#include "PRUManager.h"

namespace SourceManager {
  extern PRUManager PRU;

  void startup();
  void stop();

}
#endif
