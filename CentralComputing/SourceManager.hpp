#ifndef SOURCE_MANAGER_HPP
#define SOURCE_MANAGER_HPP

#include "PRUManager.hpp"
#include "CANManager.hpp"
#include "TMPManager.hpp"
#include "ADCManager.hpp"
#include "I2CManager.hpp"
#include "MotionModel.hpp"

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
