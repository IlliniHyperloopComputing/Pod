#ifndef SIM
#include "Sensor.h"

Sensor::Sensor(Spi * s, Battery * b) : spi(s), battery(b) {
  //setup maps
  raw_data_map = raw_data_map_t();
  calculation_map = calculation_map_t();
  parse_map = parse_map_t();

  // Distance
  Arbitrary_Data distance = {
    sizeof(Distance_Raw),
    (uint8_t *) malloc(sizeof(Distance_Raw))
  };
  raw_data_map[Data_ID::DISTANCE] = distance;
  calculation_map[Data_ID::DISTANCE] = distance_calculation;
  parse_map[Data_ID::DISTANCE] = distance_parse;

  //AccelerationX
  Arbitrary_Data acceleration_x= {
    sizeof(Acceleration_X_Raw),
    (uint8_t *) malloc(sizeof(Acceleration_X_Raw))
  };
	raw_data_map[Data_ID::ACCELERATION_X] = acceleration_x;
	calculation_map[Data_ID::ACCELERATION_X] = acceleration_x_calculation;
	parse_map[Data_ID::ACCELERATION_X] = acceleration_x_parse;

  //AccelerationY
  Arbitrary_Data acceleration_y= {
    sizeof(Acceleration_Y_Raw),
    (uint8_t *) malloc(sizeof(Acceleration_Y_Raw))
  };
	raw_data_map[Data_ID::ACCELERATION_Y] = acceleration_y; 
	calculation_map[Data_ID::ACCELERATION_Y] = no_calculation;
	parse_map[Data_ID::ACCELERATION_Y] = acceleration_y_parse;

  //AccelerationZ
  Arbitrary_Data acceleration_z= {
    sizeof(Acceleration_Z_Raw),
    (uint8_t *) malloc(sizeof(Acceleration_Z_Raw))
  };
	raw_data_map[Data_ID::ACCELERATION_Z] = acceleration_z; 
	calculation_map[Data_ID::ACCELERATION_Z] = no_calculation;
	parse_map[Data_ID::ACCELERATION_Z] = acceleration_z_parse;

}

Data Sensor::get_data(Data_ID id){
  Arbitrary_Data raw_data = get_raw_data(id);
  calculation_func_t f = calculation_map[id];
  Arbitrary_Data calculated = f(raw_data);
  Data d;
  d.calculated = calculated;
  d.raw = raw_data;
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
  Xmega_Transfer transfer = {XMEGA1, X_C_NONE, X_R_ALL};
  uint8_t status = spi->transfer(transfer);
  if(status != X_TF_NONE){
    //Do something if error, like count how many misses
  }
  else{
    //Do something if not error
    //If sending command, don't need to resend
  }

  transfer.device = XMEGA2;
  status = spi->transfer(transfer);
  if(status != X_TF_NONE){
    //Do something if error, like count how many misses
  }
  else{
    //Do something if not error
    //If sending command, don't need to resend
  }


  //
  // CAN BUS GOES HERE PROBABLY MAYBE
  //
  
  //update from sources
  //if I were cool, I would have made Spi and Data_ID be subclasses of some kind of Source class but 
  //I'm far too lazy for that. Void Pointers to the rescue!

  for(int i = 0; i < Data_ID::MOTOR_INFO; i++){
    Data_ID id = (Data_ID)i;
    parse_func_t fun = parse_map[id];
    Arbitrary_Data raw = raw_data_map[id];
    fun(spi, raw);
  }
  //gotta skip over motor info/state info ugh
  for(int j = Data_ID::BATTERY_FLAGS; j <= Data_ID::BATTERY_INFO; j++){
    //Then, each parse function should just read from the file
    Data_ID id = (Data_ID)j;
    parse_func_t fun = parse_map[id];
    Arbitrary_Data raw = raw_data_map[id];
    fun(battery, raw);
  }
}

#endif
