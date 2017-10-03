#include "Sensor.h"


Data * Sensor::get_data(Data_ID id){
  Raw_Data * raw = get_raw_data(id);
  calculation_func_t f = calculation_map[id];
  Data * data = new Data();
  data->actual = f(raw);
  data->raw = raw;
  return data;
}

Raw_Data * Sensor::get_raw_data(Data_ID id){
  sensor_mutex.lock();
  Raw_Data * raw = new Raw_Data(*raw_data_map[id]);
  sensor_mutex.unlock();
  return raw;
}
