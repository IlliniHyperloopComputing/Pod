#include "Command.h"

SafeQueue<uint16_t> Command::command_queue;
int64_t Command::error_flag_timers[8*6]; 

void Command::put(uint8_t id, uint8_t value) {
  uint16_t toQueue = (id << 8) | value;
  command_queue.enqueue(toQueue);
}

bool Command::get(Network_Command * com) {
  uint16_t preShift;
  if (!command_queue.dequeue(&preShift)) {
    return false;
  }
  com->value = static_cast<uint8_t>(preShift);
  com->id = static_cast<uint8_t>(preShift >> 8);
  return true;
}

// Used in set_error_flag to not flood the command queue
// See the .h for more explanation
void Command::set_error_flag(uint8_t id, uint8_t value) {
  // Initialize if this is the first function call
  static bool first_time = 1;
  if (first_time) {
    // Setup error flag timers to -1
    for (int i = 0; i < 8*6; i++) {
      Command::error_flag_timers[i] = -1000000;  // negative 1 second. 
    }
    first_time = 0;
  }

  for (int i = 0, j = 1; i < 8; i++, j*=2) {  // Go through each bit of the flag  
    if (value & j) {  // if the specific bit is on
      // Determine the time delta, use the error_flag_timers
      int64_t delta = Utils::microseconds() - error_flag_timers[(id-Command::SET_ADC_ERROR) * 8 + i]; 
      if (delta > 1000000) {  // Delta is greater than 1 second. This means we only send once per second!
        error_flag_timers[(id-Command::SET_ADC_ERROR) * 8 + i] = Utils::microseconds();
        Command::put(id, value & j);  // put command on queue
      }
    }
  }
}

