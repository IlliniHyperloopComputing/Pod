#include "Battery.h"


Battery::Battery(std::string _filename) : filename(_filename){
  // TODO open the file
}

Arbitrary_Data Battery::get_raw_data(int id){
  Data_ID d_id = (Data_ID) id; //THIS IS SO SO DUMB. I need to make the function take in a data id, but there is a class cycle so I need to figure out a better place to put Data_ID so the whole thing doesn't shit the bed, but I'm too lazy to fix it now
  if(id < 7){
    print_info("Attempting to request bad data, abort");
  }
  Arbitrary_Data d;
  switch(d_id) {
    case BATTERY_FLAGS:
      {// fill in from saved struct
      uint8_t * data = (uint8_t *)malloc(sizeof(flags));
      memcpy(data, &flags, sizeof(flags));
      d.data = data; 
      d.size = sizeof(flags);
      break;
      }
    case BATTERY_VOLT:
      // do something
      break;
    case BATTERY_AMP:
      // do something
      break;
    case BATTERY_TEMP:
      // do something
      break;
    case BATTERY_OHM:
      // do something
      break;
    case BATTERY_INFO:
      // do something
      break;
    default:
      break;
  }
  return d;
}

void Battery::refresh_data(){
  //find a way to read only the n most recent pieces of data, maybe start at the bottom?
  file.open(filename);
  while(!file.eof()){
    string can, index, bytes, rest;
    file >> can >> index >> bytes;
    bytes = bytes.substr(1, bytes.size() -2);
    uint32_t size = stoi(bytes);
    uint32_t messageType = stoi(index);
    uint8_t * data = (uint8_t *) malloc(size);
    for(int j = 0; j < size; j++){
      string byte;
      file >> byte;
      data[j] = std::stoi(byte, 0, 16);
    }
    //TODO parse the strings/data and store in the saved struct
  }
   
}
