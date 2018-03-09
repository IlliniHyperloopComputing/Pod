#ifndef BATTERY_H
#define BATTERY_H

#include "Utils.h"
#include "Data.h"
#include "Sensor.h"
#include <iostream>
#include <fstream>
struct Battery_Amp_Raw {
};
struct Battery_Flags_Raw {
};
struct Battery_Info_Raw {
  uint32_t energy_in;
  uint32_t energy_out;
};
struct Battery_Ohm_Raw {

  uint8_t cell[8];
};
struct Battery_Volt_Raw {
  uint8_t cell[8];
};
struct Battery_Temp_Raw {
  uint8_t cell[8];
};

class Battery {
  public:
  /**
  * Constructs a battery object, opens the file
  **/
  Battery(std::string filename);
 
  /**
  * Reads from the file to get the raw data associated with the data id
  * @param id the id
  * @return an arbitrary data struct that has the raw data for that item
  */
  Arbitrary_Data get_raw_data(int id);

  /**
  * Dumps data from the canbus into the file, updating its contents
  */
  void refresh_data();
  
  private:
  std::string filename;
  ifstream file;
  Battery_Volt_Raw volt;
  Battery_Amp_Raw amp;
  Battery_Temp_Raw temp;
  Battery_Ohm_Raw ohm;
  Battery_Info_Raw info;
  Battery_Flags_Raw flags;
};

#endif
