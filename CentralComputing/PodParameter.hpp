#ifndef PODPARAM_H
#define PODPARAM_H

#include <iostream>
#include <vector>
#include "SourceManager.h"

using namespace std;

template <class Type>
class Param {
  public:
    Type Value;
    long Timestamp;
};

template <class Type>
class PodParameter {
  public:
    virtual Param<Type> Get() = 0;

    vector<uint8_t> GetNetworkData() {
      vector<uint8_t> vec(sizeof(Type)); 
      Param<Type> val = Get();
      memcpy(vec.data(), &(val.Value), sizeof(Type));
      return vec;
    }
  //we could add functions for storing the most recent data to do filtering/moving average
  //we could include timestamping for 
};

#endif
