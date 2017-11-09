#include "Current.h"

#define CURRENT_OFFSET 0

using namespace std;

void current_parse(uint8_t * buffer, Arbitrary_Data raw) {
	Current_Raw * cur_raw = (Current_Raw *)raw.data;
	cur_raw->current = *(((uint16_t *)buffer) + CURRENT_OFFSET);
}
