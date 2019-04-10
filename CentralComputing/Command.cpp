#include "Command.h"

void Command::put(uint8_t ID, uint8_t Command){
	uint16_t toQueue = (ID << 8) | Command;
	command_queue.enqueue(toQueue);
}

bool Command::get(uint8_t & ID, uint8_t & Command){
	bool loaded = false;
	uint16_t preShift = command_queue.dequeue();
	if (!preShift) return loaded;
	Command = static_cast<uint8_t>(preShift);
	ID = static_cast<uint8_t>(preShift >> 8);
	loaded = true;
	return loaded;
}
