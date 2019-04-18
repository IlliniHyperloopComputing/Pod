#include "Command.h"

SafeQueue<uint16_t> Command::command_queue;

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
