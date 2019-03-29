#ifndef SOURCE_MANAGER_HPP
#define SOURCE_MANAGER_HPP

#include "PRUManager.h"
#include "CANManager.h"
#include "TMPManager.h"
#include "ADCManager.h"
#include "I2CManager.h"
#include "MotionModel.h"

// Include stubbs to allow Managers to 
// include this .hpp in their files
class PRUManager;
class CANManager;
class TMPManager;
class ADCManager;
class I2CManager;
class MotionModel;

namespace SourceManager {
  extern PRUManager PRU;
  extern CANManager CAN;
  extern TMPManager TMP;
  extern ADCManager ADC;
  extern I2CManager I2C;
  extern MotionModel MM;
}
#endif
