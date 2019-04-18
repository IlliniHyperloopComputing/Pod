#include "Command.h"

SafeQueue<uint16_t> Command::command_queue;

void Command::put(uint8_t ID, uint8_t Command) {
  uint16_t toQueue = (ID << 8) | Command;
  command_queue.enqueue(toQueue);
}

bool Command::get(Network_Command * com) {
  uint16_t preShift;
  bool loaded = command_queue.dequeue(&preShift);
  if (!loaded) return loaded;
  com->id = static_cast<uint8_t>(preShift);
  com->value = static_cast<uint8_t>(preShift >> 8);
  return loaded;
}
