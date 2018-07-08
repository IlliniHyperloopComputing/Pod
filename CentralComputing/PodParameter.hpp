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

    vector<uint8_t> GetNetworkData() {
      vector<uint8_t> vec(sizeof(Type)); 
      shared_ptr<Type> val = Get();
      memcpy(vec.data(), val, sizeof(Type));
      return vec;
    }
  //we could add functions for storing the most recent data to do filtering/moving average
  //we could include timestamping for 
};

#endif
