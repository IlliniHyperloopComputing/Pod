#ifndef SIM
#include "Sensor.h"

Sensor::Sensor(Xmega * xmega) : xmega(xmega){

}

Data * Sensor::get_data(Data_ID id){
  Arbitrary_Data raw_data = get_raw_data(id);
  calculation_func_t f = calculation_map[id];
  Arbitrary_Data calculated = f(raw_data);
  Data * d = (Data *) malloc(sizeof(Data));
  d->calculated = calculated;
  d->raw = raw_data;
  return d;

}

Arbitrary_Data Sensor::get_raw_data(Data_ID id){
  sensor_mutex.lock();
  Arbitrary_Data raw = get_raw_data(id);
  raw.size = raw_data_map[id].size;
  raw.data = (uint8_t *) malloc(raw.size);
  memcpy(raw.data, raw_data_map[id].data, raw.size);
  return raw;
}

void Sensor::update_buffers() {
  //update from Xmega

  for(Data_ID id : ids){
    parse_func_t fun = parse_map[id];
    Arbitrary_Data raw = raw_data_map[id];
    fun(data_buffer, raw); 
  }
}

#endif
