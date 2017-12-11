#include "Current.h"

#define CURRENT_OFFSET 0

using namespace std;

void current_parse(Spi * spi, Arbitrary_Data raw) {
	Current_Raw * cur_raw = (Current_Raw *)raw.data;
	cur_raw->current = spi->get_data(XMEGA2, CURRENT_INDEX_1);
}
