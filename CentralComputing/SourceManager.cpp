#include "SourceManager.hpp"

PRUManager SourceManager::PRU;
CANManager SourceManager::CAN;
TMPManager SourceManager::TMP;
ADCManager SourceManager::ADC;
I2CManager SourceManager::I2C;
MotionModel SourceManager::MM;

Event SourceManager::PRU_update;
Event SourceManager::ADC_update;

