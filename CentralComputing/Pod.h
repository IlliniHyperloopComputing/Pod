#ifndef POD_H_
#define POD_H_

#include "TCPManager.h"
#include "Command.h"
#include "UDPManager.h"
#include "Event.h"
#include "Pod_State.h"
#include "Configurator.h"
#include "MotionModel.h"
#include "SourceManager.h"
#include "Defines.hpp"
#include <string>
#include <functional>
#include <memory>
#include <semaphore.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#include "gtest/gtest.h"
#pragma GCC diagnostic pop

class Pod {
 public:
  explicit Pod(const std::string & config_to_open);
  ~Pod();

  void run();
  void trigger_shutdown();

  std::shared_ptr<Pod_State> state_machine;
  std::shared_ptr<MotionModel> motion_model;
  std::atomic<bool> running;
  std::shared_ptr<UnifiedState> unified_state;
  Event ready;
  Event processing_command;
  Event closing;

 private:
  void logic_loop();
  void update_unified_state();
  // IF the command is an error command, set the unified state appropriatly
  void set_error_code(Command::Network_Command * com);    
  bool switchVal;
  string tcp_port;
  string tcp_addr;
  string udp_send;  // port we send packets to
  string udp_recv;  // port we recv packets from
  string udp_addr; 
  int64_t logic_loop_timeout;  // logic_loop sleep (timeout) value
};

namespace podtest_global {
  extern std::string config_to_open;
}  // namespace podtest_global

void signal_handler(int signal);

#endif  // POD_H_
