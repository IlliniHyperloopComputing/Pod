#ifndef SOURCE_MANAGER_HPP
#define SOURCE_MANAGER_HPP

#include "PRUManager.hpp"
#include "CANManager.hpp"
#include "TMPManager.hpp"
#include "ADCManager.hpp"
#include "I2CManager.hpp"

namespace SourceManager {
  extern PRUManager PRU;
  extern CANManager CAN;
  extern TMPManager TMP;
  extern ADCManager ADC;
  extern I2CManager I2C;
  extern std::string DUMMY;
}
#endif
