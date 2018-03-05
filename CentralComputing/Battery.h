#ifndef BATTERY_H
#define BATTERY_H

#include "Sensor.h"

class Battery {
  public:
  /**
  * Constructs a battery object, opens the file
  **/
  Battery(char * filename);
 
  /**
  * Reads from the file to get the raw data associated with the data id
  * @param id the id
  * @return an arbitrary data struct that has the raw data for that item
  */
  Arbitrary_Data get_raw_data(Data_ID id);

  /**
  * Dumps data from the canbus into the file, updating its contents
  */
  void refresh_data();
  
  private:
  FILE * file;
};
#endif
