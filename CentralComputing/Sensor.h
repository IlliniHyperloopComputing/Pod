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
  BRAKE_PRESSURE_INDEX = 3,
  ENCODER_RPM_INDEX = 4,
  ENCODER_COUNT_INDEX = 5,
};

enum Xmega_2_Indices {
  Y_ACCELERATION_INDEX = 0,
  Z_ACCELERATION_INDEX = 1,
  RIDE_HEIGHT_INDEX_1 = 2,
  RIDE_HEIGHT_INDEX_2 = 3,
  RIDE_HEIGHT_INDEX_3 = 4,
  BATTERY_CELL_INDEX_1 = 5,
  BATTERY_CELL_INDEX_2 = 6,
  THERMOCOUPLE_INDEX_1 = 7,
  THERMOCOUPLE_INDEX_2 = 8,
  THERMOCOUPLE_INDEX_3 = 9,
  THERMOCOUPLE_INDEX_4 = 10,
  THERMOCOUPLE_INDEX_5 = 11,
  CURRENT_INDEX_1 = 12,
  CURRENT_INDEX_2 = 13,
  TAPE_COUNT_INDEX = 14
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
	VOLTAGE = 5,
	CURRENT = 6,
	BRAKE_PRESSURE = 7,
  TEMPERATURE = 8,
  RIDE_HEIGHT = 9,
  //etc etc
  STATE_ID = 10 // State ID is an invalid ID to request, query the state machine instead
};

//You can think of Arbitrary Data as a uint8_t * in most circumstances, it just also gives a size

// A calculation function takes in a pointer to raw data and converts it to real units
typedef Arbitrary_Data (*calculation_func_t)(Arbitrary_Data);
// A parse function takes in a spi reference, and stores necessary data within a Raw_Data_Struct 
typedef void (*parse_func_t)(Spi * spi, Arbitrary_Data data);
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

  private:
    #ifdef SIM

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
