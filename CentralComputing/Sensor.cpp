#ifndef SIM
#include "Sensor.h"
#include "Sensor_Aux/Distance.h"
#include "Sensor_Aux/Temperature.h"
#include "Sensor_Aux/Ride_Height.h"
#include "Sensor_Aux/Null.h"
#include "Sensor_Aux/Acceleration_X.h"
#include "Sensor_Aux/Acceleration_Y.h"
#include "Sensor_Aux/Acceleration_Z.h"
#include "Sensor_Aux/Current.h"
#include "Sensor_Aux/Brake_Pressure.h"

Sensor::Sensor(Spi * s) : spi(s){
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

  //Temperature
  Arbitrary_Data temperature = {
    sizeof(Temperature_Raw),
    (uint8_t *) malloc(sizeof(Temperature_Raw))
  };
  raw_data_map[Data_ID::TEMPERATURE] = temperature;
  calculation_map[Data_ID::TEMPERATURE] = no_calculation;
  parse_map[Data_ID::TEMPERATURE] = temperature_parse;

  //Ride Height
  Arbitrary_Data ride_height= {
    sizeof(Ride_Height_Raw),
    (uint8_t *) malloc(sizeof(Ride_Height_Raw))
  };
  raw_data_map[Data_ID::RIDE_HEIGHT] = ride_height; 
  calculation_map[Data_ID::RIDE_HEIGHT] = no_calculation;
  parse_map[Data_ID::RIDE_HEIGHT] = ride_height_parse;

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

  //Current 
  Arbitrary_Data current= {
    sizeof(Current_Raw),
    (uint8_t *) malloc(sizeof(Current_Raw))
  };
	raw_data_map[Data_ID::CURRENT] = current; 
	calculation_map[Data_ID::CURRENT] = no_calculation;
	parse_map[Data_ID::CURRENT] = current_parse;

  //Brake Pressure
  Arbitrary_Data brake_pressure = {
    sizeof(Brake_Pressure_Raw),
    (uint8_t *) malloc(sizeof(Brake_Pressure_Raw))
  };
	raw_data_map[Data_ID::BRAKE_PRESSURE] = brake_pressure;
	calculation_map[Data_ID::BRAKE_PRESSURE] = no_calculation;
	parse_map[Data_ID::BRAKE_PRESSURE] = brake_pressure_parse;




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
    //Do something if error
  }
  else{
    //Do something if not error
    //If sending command, don't need to resend
  }

  transfer.device = XMEGA2;
  status = spi->transfer(transfer);
  if(status != X_TF_NONE){
    //Do something if error
  }
  else{
    //Do something if not error
    //If sending command, don't need to resend
  }


  //
  // CAN BUS GOES HERE PROBABLY MAYBE
  //

  for(Data_ID id : ids){
    parse_func_t fun = parse_map[id];
    Arbitrary_Data raw = raw_data_map[id];
    fun(spi, raw);
  }
}

#endif
