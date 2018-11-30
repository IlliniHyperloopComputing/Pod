#ifndef MOTION_MODEL_HPP
#define MOTION_MODEL_HPP

//not sure what I need for log levels so I put a lot
#include "Utils.h"
#include "SourceManager.hpp"
#include "Event.hpp"
#include "Pod.h"
#include "Simulator.hpp"
#include <iostream>
#include <cstdio>
#include <sched.h>
#include <map>

class ConfigManager{
  public:
    void load(std::string fileName);
    std::map<std::string, double> constants;
};




#endif
