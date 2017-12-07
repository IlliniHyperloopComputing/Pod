#include "Voltage.h"



using namespace std; 

void voltage_parse(Spi * spi, Arbitrary_Data raw) {
  Voltage_Raw * v_raw = (Voltage_Raw *)raw.data;                           // Convert the raw data into a useable struct
  v_raw->voltage = spi->get_data(XMEGA2, VOLTAGE_INDEX_1);
  
}
