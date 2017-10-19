#ifndef SENSOR_H
#define SENSOR_H

#include "Xmega.h"
#include <map>
#include <string.h>
#include "Data.h"
#include <mutex>
#include <vector>

/**Every Data_ID needs:
  1. A Raw_Data_Struct that holds all the relevant raw data for that query
  2. A Calculated_Data_Struct that holds all the calculated data
  3. A Calculation Function to convert the relevant raw data to a "true" or "actual" value
  4. A Parsing Function to pull the important data out of the data buffers and update the raw_data 

  Some Data_ID require:
  A function to convert calculated data 
**/
enum Data_ID {
  DISTANCE,
  VELOCITY,
  ACCELERATION,
  TEMPERATURE,
  RIDE_HEIGHT,
  //etc etc
  NULL_ID
};

//You can think of Arbitrary Data as a uint8_t * in most circumstances, it just also gives a size

// A calculation function takes in a pointer to raw data and converts it to real units
typedef Arbitrary_Data (*calculation_func_t)(Arbitrary_Data);
// A parse function takes in a pointer to a buffer, and memcpys the relevant data onto an existing arbitrary data
typedef void (*parse_func_t)(uint8_t * buffer, Arbitrary_Data data);
// Calculation  map takes in a Data_ID and gives a calculation function
typedef std::map<Data_ID, calculation_func_t> calculation_map_t;
// Raw data map maps a data_id to some raw_data *
typedef std::map<Data_ID, Arbitrary_Data > raw_data_map_t;
// Parse function map maps a Data_ID to a parsing function
typedef std::map<Data_ID, parse_func_t> parse_map_t;


class Sensor { 
  public:
     
    /**
    * Default constructor
    **/
    Sensor(Xmega * xmega);

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
    Data get_data(Data_ID id);

  private: 
    #ifdef SIM

    #else 
      Xmega * xmega;
      calculation_map_t calculation_map;
      raw_data_map_t raw_data_map;
      parse_map_t parse_map;


      Arbitrary_Data get_raw_data(Data_ID id);
      std::vector<Data_ID> ids;
      uint8_t * data_buffer;

    #endif

    std::mutex sensor_mutex;

};

#endif
