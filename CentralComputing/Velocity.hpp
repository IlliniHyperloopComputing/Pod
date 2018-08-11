#ifndef VELOCITY_H
#define VELOCITY_H

#include "PodParameter.hpp"
#include "Filter.h"
#include <algorithm>

#define NUM_VEL_INPUTS 4

using namespace Utils;

class Velocity : public PodParameter<double> {
  public:
    
    int    velocity_idx[NUM_VEL_INPUTS] = {1, 2, 3, 4}; 
    double distance[NUM_VEL_INPUTS] = {1, 1, 1, 1};
    double velocities[NUM_VEL_INPUTS] = {0, 0, 0, 0};
    double velocity_output = 0;

    shared_ptr<double> Get() {
      // Create variable we are going to return
      shared_ptr<double> p = make_shared<double>(); 
//
//      // Grab decays and deltas from PRU
//      auto pru_data = SourceManager::PRU.Get();
//      uint32_t * decays = pru_data->decays;
//      uint32_t * deltas = pru_data->deltas;
//
//      // Strategy: For the sensors we are interested in, use
//      //           the sensor data that provides the lowest
//      //           velocity. Low pass filter. Then take median
//      
//      for(int i = 0; i < NUM_VEL_INPUTS; i++){
//        uint32_t decay = decays[velocity_idx[i]];   
//        uint32_t delta = deltas[velocity_idx[i]];   
//
//        // Pick the one that gives us the slower velocity
//        uint32_t slower = std::max(decay, delta);
//
//        double vel;
//        if(slower == UINT32_MAX){// register this as 0 velocity
//          vel = 0;
//        }
//        else{
//          // Do the proper conversion into m/s
//          double time_diff = slower * (21.474836475 / 4294967295.0);
//          vel = distance[i] / time_diff;
//        }
//
//        // Low pass filter
//        velocities[i] = Filter::LowPass(velocities[i], vel);
//      	print(LogLevel::LOG_EDEBUG, "Velocity[%d]: %f\t",i, velocities[i]);
//      }
//      print(LogLevel::LOG_EDEBUG, "\n");
//
//      // Create temporary array
//      double tmp_v[NUM_VEL_INPUTS];
//      memcpy(tmp_v, velocities, sizeof(tmp_v));
//
//      // Find the median value 
//      // Array will be modified, which is why we needed them temporary
//      std::nth_element(tmp_v, tmp_v + NUM_VEL_INPUTS/2, tmp_v + NUM_VEL_INPUTS);
//      double median = tmp_v[NUM_VEL_INPUTS/2];
//
//      print(LogLevel::LOG_EDEBUG, "Velocity Median: %f\n", median);
//      
//      // LowPass filter once more
//      velocity_output = Filter::LowPass(velocity_output, median);
//
//      print(LogLevel::LOG_EDEBUG, "Velocity Output: %f\n", velocity_output);
//
//      // Set the return value
//      *p = velocity_output;
      return p;
    }
};

#endif
