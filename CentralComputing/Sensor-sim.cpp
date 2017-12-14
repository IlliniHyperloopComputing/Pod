#ifdef SIM

#include "Sensor.h"
Sensor::Sensor(Spi * s) {

}

Data Sensor::get_data(Data_ID id){
  Arbitrary_Data a;
  a.data = (uint8_t *) malloc(sizeof(int));
  a.size = sizeof(int);
  *((int *)a.data) = (int)0xdeadbeef;
  Data d;
  d.calculated = a;
  d.raw = null_data;
  return d;
}

void Sensor::update_buffers() {
}
#endif
