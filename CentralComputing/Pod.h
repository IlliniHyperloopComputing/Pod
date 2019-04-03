#ifndef POD_H_
#define POD_H_

#include "TCPManager.h"
#include "UDPManager.h"
#include "Event.h"
#include "Pod_State.h"
#include "Configurator.h"
#include "MotionModel.h"
#include "SourceManager.h"
#include <string>
#include <functional>
#include <memory>
#include <semaphore.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "gtest/gtest.h"
#pragma GCC diagnostic pop

using std::shared_ptr;

struct UnifiedState{
  shared_ptr<MotionData> motion_data;
  shared_ptr<ADCData> adc_data;
  shared_ptr<CANData> can_data;
  shared_ptr<I2CData> i2c_data;
  shared_ptr<PRUData> pru_data;
  Pod_State::E_States state;
};

class Pod {
 public:
  Pod();

  void run();
  void trigger_shutdown();

  std::shared_ptr<Pod_State> state_machine;
  std::atomic<bool> running;
  UnifiedState unified_state;
  Event ready;
  Event processing_command;
  Event closing;

 private:
  void logic_loop();
  void update_unified_state();
  bool switchVal;
};

void signal_handler(int signal);

#endif  // POD_H_
