#ifndef SENSOR_H
#define SENSOR_H

#include "Xmega.h"
#include <map>
#include <string.h>
#include "Data.hpp"
#include <mutex>

//a data ID is the identifier for a query
/**Every Data_ID needs:
  1. A Raw_Data struct that holds all the relevant raw data for that query
  2. A Calculation Function to convert the relevant raw data to a "true" or "actual" value
  3. A Parsing Function to pull the important data out of the data buffers and update the raw_data 
  4. A Network Function to convert the Data and Raw_Data into a network buffer to be sent
**/
enum Data_ID {
  DISTANCE,
  VELOCITY,
  ACCELERATIONX,
  ACCELERATIONY,
  ACCELERATIONZ,
  TEMPERATURE,
  RIDE_HEIGHT,
  //etc etc
};

// A calculation function takes in a pointer to raw data and converts it to real units
typedef double (*calculation_func_t)(Raw_Data *);
typedef Raw_Data * (*parse_func_t)(uint8_t *);
// Calculation  map takes in a Data_ID and gives a calculation function
typedef std::map<Data_ID, calculation_func_t> calculation_map_t;
// Raw data map maps a data_id to a raw_data *
typedef std::map<Data_ID, Raw_Data *> raw_data_map_t;


class Sensor { 
  public:
    
    /**
    * Updates the buffers with the most recent data
    * Note: may not do anything
    * Note: dirty bit/timers
    * Note: maybe should be private? **/
    void update_buffers();

    /**
    * Returns a pointer to a struct corresponding to the scaled data for the Sensor_ID
    * Note: it is up to the user to match the returned struct with the right data
    * @param id the id of the sensor to be queried
    * @return data the data associated with the sensor
    */
    Data * get_data(Data_ID id);

  private: 
    #if SIMULATION

    #else
      Xmega * xmega;
      calculation_map_t calculation_map;
      raw_data_map_t raw_data_map;
      Raw_Data * get_raw_data(Data_ID id);
    #endif
    std::mutex sensor_mutex;

};

#endif
