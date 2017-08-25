#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Optical::Optical(Sensor_Configuration configuration) : Sensor_Group(configuration) {
  
  first_index = OPTICAL_INDEX; // index offset to read from spi
  device = XMEGA1; //xmega device number (0 or 1)
  count = 3; //number of sensors
    translation_array = {{RPM_TRANS,NO_TRANS, NO_TRANS}};
    name = "Optical";
    name_array = {{"RPM", "Count"}};
  data = vector<double>(count);   
}

Optical::~Optical(){
  //do nothing
}

void Optical::reset() {
  //TODO: implement resetting
}

void Optical::update(Spi * spi) {

  switch(simulation) {
    case 0:
      refresh_data(spi);
      if(data[2] > 500) { 
        data[0] = 0;

      }
      break;
    case 1:
      simulation_1();
      break;
    case 2:
      simulation_2();
      break;
  }
}

void Optical::simulation_1() {
  
  
  sensor_group_mutex.lock();
  /*
  for(size_t i = 0; i < data.size(); i++) {
    data[i] = i;
  }*/
  data[0] = 450;
  data[1] = 400;
  data[2] = 0;
  sensor_group_mutex.unlock();
}

void Optical::simulation_2() {
  
  auto start = Sensor_Package::start_time;
  auto now = Sensor_Package::get_current_time();
  auto difference = now - start;
  sensor_group_mutex.lock();
  data[2] = 0;
  if(difference < 30 * 1000){
    data[0] = 0;
    data[1] = 0;
  } else if(difference < 45 * 1000) {
    data[0] = difference/1000;
  } else if(difference < 60 * 1000) {
    
  } else {
    data[1] = 400; 
  }
  
  sensor_group_mutex.unlock();
}


uint8_t *  Optical::get_data_buffer() {
  uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());
  sensor_group_mutex.lock();
  
  memcpy(buffer, &data[0], sizeof(double));
  uint32_t value = data[1];
  memcpy(buffer + sizeof(double), &value, sizeof(uint32_t));
  sensor_group_mutex.unlock();
  
  return buffer;
}

size_t Optical::get_buffer_size() {
  // double + uint32_t; 
  return sizeof(double) + sizeof(uint32_t);
}

bool Optical::greenlight() {
  return true;
}
