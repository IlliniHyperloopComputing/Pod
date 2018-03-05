#ifndef SENSOR_H
#define SENSOR_H

#include "Xmega.h"
#include <map>
#include <string.h>
#include "Data.h"
#include <mutex>
#include <vector>

#define XMEGA1 0
#define XMEGA2 1

enum Xmega_1_Indices {
  X_ACCELERATION_INDEX_1 = 0,
  X_ACCELERATION_INDEX_2 = 1,
  X_ACCELERATION_INDEX_3 = 2,
  Y_ACCELERATION_INDEX = 3,
  Z_ACCELERATION_INDEX = 4,
  //GARBAGE STUFF
  BRAKE_PRESSURE_INDEX = 90,
  ENCODER_RPM_INDEX = 91,
  ENCODER_COUNT_INDEX = 92,
};

enum Xmega_2_Indices {
  TACH1_LAST_TIME_INDEX = 0,
  TACH2_LAST_TIME_INDEX = 1,
  TACH3_LAST_TIME_INDEX = 2,
  TACH4_LAST_TIME_INDEX = 3,
  OPT_LAST_TIME_INDEX = 4,
  TACH1_DELTA_INDEX = 5,
  TACH2_DELTA_INDEX = 6,
  TACH3_DELTA_INDEX = 7,
  TACH4_DELTA_INDEX = 8,
  OPT_ENCODER_DELTA = 9,
  OPT_ENCODER_COUNT = 10,
//GARBAGE STUFF
  RIDE_HEIGHT_INDEX_1 = 92,
  RIDE_HEIGHT_INDEX_2 = 93,
  RIDE_HEIGHT_INDEX_3 = 94,
  BATTERY_CELL_INDEX_1 = 95,
  BATTERY_CELL_INDEX_2 = 96,
  THERMOCOUPLE_INDEX_1 = 97,
  THERMOCOUPLE_INDEX_2 = 98,
  THERMOCOUPLE_INDEX_3 = 99,
  THERMOCOUPLE_INDEX_4 = 910,
  THERMOCOUPLE_INDEX_5 = 911,
  CURRENT_INDEX_1 = 912,
  CURRENT_INDEX_2 = 913,
  TAPE_COUNT_INDEX = 914
};

/**Every Data_ID needs:
  1. A Raw_Data_Struct that holds all the relevant raw data for that query
  2. A Calculated_Data_Struct that holds all the calculated data
  3. A Calculation Function to convert the relevant raw data to a "true" or "actual" value
  4. A Parsing Function to pull the important data out of the data buffers and update the raw_data

  Some Data_ID require:
  A function to convert calculated data
**/
enum Data_ID {
  DISTANCE = 0,
  VELOCITY = 1,
  ACCELERATION_X = 2,
	ACCELERATION_Y = 3,
	ACCELERATION_Z = 4,
  MOTOR_INFO = 5,
  STATE_INFO = 6, // State ID is an invalid ID to request, query the state machine instead
  BATTERY_FLAGS = 7,
  BATTERY_VOLT = 8,
  BATTERY_AMP = 9,
  BATTER_TEMP = 10,
  BATTERY_OHM = 11,
  BATTERY_INFO  = 12,
//leftover states
	VOLTAGE = 13,
	CURRENT = 14,
	BRAKE_PRESSURE = 15,
  TEMPERATURE = 16,
  RIDE_HEIGHT = 17,
};

//You can think of Arbitrary Data as a uint8_t * in most circumstances, it just also gives a size

// A calculation function takes in a pointer to raw data and converts it to real units
typedef Arbitrary_Data (*calculation_func_t)(Arbitrary_Data);
// A parse function takes in a source reference, and stores necessary data within a Raw_Data_Struct 
typedef void (*parse_func_t)(void * source, Arbitrary_Data data);
// Calculation  map takes in a Data_ID and gives a calculation function
typedef std::map<Data_ID, calculation_func_t> calculation_map_t;
// Raw data map maps a data_id to some raw_data *
typedef std::map<Data_ID, Arbitrary_Data > raw_data_map_t;
// Parse function map maps a Data_ID to a parsing function
typedef std::map<Data_ID, parse_func_t> parse_map_t;
// Data map, only used in Simulation
typedef std::map<Data_ID, Data > data_map_t; 


class Sensor {
  public:

    /**
    * Default constructor
    **/
    Sensor(Spi * s);

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


    #ifdef SIM
      void parse_sim(Data_ID id, char * data);
    #endif

  private:
    #ifdef SIM
      data_map_t data_map; 
      fd_set readfds;
      struct timeval timeout;
    #else
      Spi * spi;
      calculation_map_t calculation_map;
      raw_data_map_t raw_data_map;
      parse_map_t parse_map;


      Arbitrary_Data get_raw_data(Data_ID id);
      std::vector<Data_ID> ids;

    #endif

    std::mutex sensor_mutex;

};

#endif
