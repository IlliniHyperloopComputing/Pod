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
