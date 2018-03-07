#ifdef SIM

#include "Sensor.h"
#include "Sensor_Aux/Distance.h"
#include "Sensor_Aux/Velocity.h"
#include "Sensor_Aux/Temperature.h"
#include "Sensor_Aux/Null.h"
#include "Sensor_Aux/Acceleration_X.h"
#include "Sensor_Aux/Acceleration_Y.h"
#include "Sensor_Aux/Acceleration_Z.h"

#define MAX_LINES 10

Sensor::Sensor(Spi * s) {

  FD_ZERO(&readfds);
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;



  data_map = data_map_t();
  
  //Distance
  Data distance = {
    {
      sizeof(Distance_Calc),
      (uint8_t *) malloc(sizeof(Distance_Calc))
    },
    {
      sizeof(Distance_Raw),
      (uint8_t *) malloc(sizeof(Distance_Raw))
    }
  };
  data_map[Data_ID::DISTANCE] = distance;

  // Velocity
  Data velocity = {
    {
      sizeof(Velocity_Calc),
      (uint8_t *) malloc(sizeof(Velocity_Calc))
    },
    {
      sizeof(Velocity_Raw),
      (uint8_t *) malloc(sizeof(Velocity_Raw))
    }
  };
  data_map[Data_ID::VELOCITY] = velocity;
  Data acceleration_x = {
    {
      sizeof(Acceleration_X_Calc),
      (uint8_t *) malloc(sizeof(Acceleration_X_Calc))
    },
    {
      sizeof(Acceleration_X_Raw),
      (uint8_t *) malloc(sizeof(Acceleration_X_Raw))
    }
  };
  data_map[Data_ID::ACCELERATION_X] = acceleration_x;
  Data acceleration_y = {
    null_data,
    {
      sizeof(Acceleration_Y_Raw),
      (uint8_t *) malloc(sizeof(Acceleration_Y_Raw))
    }
  };
  data_map[Data_ID::ACCELERATION_Y] = acceleration_y;
  Data acceleration_z = {
    null_data,
    {
      sizeof(Acceleration_Z_Raw),
      (uint8_t *) malloc(sizeof(Acceleration_Z_Raw))
    }
  };
  data_map[Data_ID::ACCELERATION_Z] = acceleration_z;
  /*Data voltage = {
    null_data,
    {
      sizeof(Voltage_Raw),
      (uint8_t *) malloc(sizeof(Voltage_Raw))
    }
  };
  data_map[Data_ID::VOLTAGE] = voltage;*/
  Data temperature = {
    null_data,
    {
      sizeof(Temperature_Raw),
      (uint8_t *) malloc(sizeof(Temperature_Raw))
    }
  };
  data_map[Data_ID::TEMPERATURE] = temperature;

}

Data Sensor::get_data(Data_ID id){
  Data d = data_map[id];
  return d;
}

void Sensor::parse_sim(Data_ID id, char * data){
  Data d = data_map[id];
  switch(id) {
    case DISTANCE :
      {
        Distance_Calc * calc = (Distance_Calc *) d.calculated.data;
        double distance = *(double *) data;
        calc->distance = distance;
        break;
      }
    case VELOCITY :
      {
        Velocity_Calc * calc = (Velocity_Calc *) d.calculated.data;
        double velocity = *(double *) data;
        calc->velocity = velocity;
        break;     
      }
    case ACCELERATION_X :
      {
        Acceleration_X_Calc * calc = (Acceleration_X_Calc *) d.calculated.data;
        double accel = *(double *) data; 
        calc->x = accel;
        break;
      }
    default :
      break;
      
  }
}

void Sensor::update_buffers() {
  // Consume all of stdin
  // parse each value, shoving into data_map
  FD_SET(STDIN_FILENO, &readfds);
  char * line = NULL;
  size_t size = 0;
  while(select(1, &readfds, NULL, NULL, &timeout)){
    getline(&line, &size, stdin);
    // parse the line
    int i;
    char * rem = (char *) malloc(size); 
    sscanf(line, "%d %s\n", &i, rem);
    parse_sim((Data_ID)i, rem);
    free(rem);

  }
  free(line);


}
#endif
