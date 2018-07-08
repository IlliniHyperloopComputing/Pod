#include "SourceManager.h"

PRUManager SourceManager::PRU;

void SourceManager::startup() {
  PRU.initialize();
}

void SourceManager::stop() {
  PRU.stop();
}
