#ifndef SIM
#include "Sensor.h"
#include "Sensor_Aux/Distance.h"
#include "Sensor_Aux/Temperature.h"
#include "Sensor_Aux/Ride_Height.h"
#include "Sensor_Aux/Null.h"

Sensor::Sensor(Xmega * xm) : xmega(xm){
  //setup maps
  raw_data_map = raw_data_map_t();
  calculation_map = calculation_map_t();
  parse_map = parse_map_t();

  // Distance
  raw_data_map[Data_ID::DISTANCE] = *(Arbitrary_Data * ) malloc(sizeof(Distance_Raw));
  calculation_map[Data_ID::DISTANCE] = distance_calculation;
  parse_map[Data_ID::DISTANCE] = distance_parse;

    //Temperature
  raw_data_map[Data_ID::TEMPERATURE] = *(Arbitrary_Data * ) malloc(sizeof(Temperature_Raw));
  calculation_map[Data_ID::TEMPERATURE] = no_calculation;
  parse_map[Data_ID::TEMPERATURE] = temperature_parse;

    //Ride Height
  raw_data_map[Data_ID::RIDE_HEIGHT] = *(Arbitrary_Data * ) malloc(sizeof(Ride_Height_Raw));
  calculation_map[Data_ID::RIDE_HEIGHT] = no_calculation;
  parse_map[Data_ID::RIDE_HEIGHT] = ride_height_parse;
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
  Arbitrary_Data raw;
  raw.size = raw_data_map[id].size;
  raw.data = (uint8_t *) malloc(raw.size);
  memcpy(raw.data, raw_data_map[id].data, raw.size);
  sensor_mutex.unlock();
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
