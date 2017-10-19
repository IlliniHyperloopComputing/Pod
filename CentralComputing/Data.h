#ifndef DATA_H
#define DATA_H
#include <string.h>
#include <vector>
#include "Utils.h"

// a struct containing arbitrary data
// Arbitrary data structs are held in the raw_data map and updated on update_buffers
struct Arbitrary_Data {
  size_t size;
  uint8_t * data;
};

// Data structs hold calculated data and the raw data
// Data structs are returned by the get_data function
struct Data { 
  Arbitrary_Data calculated;
  Arbitrary_Data raw;
};

/**
* Appends the data in source to target, ensuring there is adequate space
* Returns the new bytes_used for target 
**/
static size_t append(Arbitrary_Data target, size_t target_used, Arbitrary_Data source){
  if(target.size == 0){
    target.size = source.size;
    target.data = (uint8_t *) malloc(source.size);
    memcpy(target.data, source.data, source.size);
    return source.size;
  } else {
    size_t total_size = target_used + source.size; 
    if(total_size > target.size){
      //too big to fit
      size_t new_size = 2*(target.size+source.size);
      target.data = (uint8_t *) realloc(target.data, new_size);
      target.size = new_size;
      memcpy(target.data + target_used, source.data, source.size);
      return target_used + source.size;
    } else {
      //append data
      memcpy(target.data + target_used, source.data, source.size);
      return target_used + source.size;

    }
  }
}

// ------------------- CLEANUP FUNCTIONS --------------
static void cleanup(Arbitrary_Data d){
  free(d.data);
}
static void cleanup(Arbitrary_Data * d){
  cleanup(*d);
  free(d);
}
static void cleanup(Data d){
  cleanup(d.calculated);
  cleanup(d.raw);
}
static void cleanup(Data * d){
  cleanup(*d);
  free(d);
}

static Arbitrary_Data null_data = {0, NULL};
#endif
