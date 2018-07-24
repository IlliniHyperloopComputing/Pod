#ifndef PODPARAM_H
#define PODPARAM_H

#include <iostream>
#include <vector>
#include "SourceManager.hpp"

using namespace std;


template <class Type>
class PodParameter {
  public:
    virtual shared_ptr<Type> Get() = 0;

    //[size][data]
    vector<uint8_t> get_network_packet() {
      vector<uint8_t> vec(4 + sizeof(Type)); 
      int size = sizeof(Type);
      memcpy(vec.data(), size, sizeof(size));
      shared_ptr<Type> val = Get();
      memcpy(vec.data() + 4, val, sizeof(Type));
      return vec;
    }
  //we could add functions for storing the most recent data to do filtering/moving average
  //we could include timestamping for 
};

#endif
