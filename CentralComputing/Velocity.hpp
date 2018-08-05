#ifndef VELOCITY_H
#define VELOCITY_H

#include "PodParameter.hpp"
#include "Filter.h"
#include <algorithm>

#define NUM_VEL_INPUTS 4

class Velocity : public PodParameter<double> {
  public:
    
    int    velocity_idx[NUM_VEL_INPUTS] = {1, 3, 5, 7}; 
    double velocities[NUM_VEL_INPUTS] = {0};
    double velocity_output = 0;

    shared_ptr<double> Get() {
      // Create variable we are going to return
      shared_ptr<double> p = make_shared<double>(); 

      // Grab decays and deltas from PRU
      auto pru_data = SourceManager::PRU.Get();
      uint32_t * decays = pru_data->decays;
      uint32_t * deltas = pru_data->deltas;

      // Strategy: For the sensors we are interested in, use
      //           the sensor data that provides the lowest
      //           velocity. Low pass filter. Then take median
      
      for(int i = 0; i < NUM_VEL_INPUTS; i++){
        uint32_t decay = decays[velocity_idx[i]];   
        uint32_t delta = deltas[velocity_idx[i]];   

        double vel = std::max(decay, delta) * (21.474836475 / 4294967295.0);

        velocities[i] = Filter::LowPass(velocities[i], vel);
      }

      // Create temporary array
      double tmp_v[NUM_VEL_INPUTS];
      memcpy(tmp_v, velocities, sizeof(tmp_v));

      // Find the median value, array will be modified
      std::nth_element(tmp_v, tmp_v + NUM_VEL_INPUTS/2, tmp_v + NUM_VEL_INPUTS);
      double median = tmp_v[NUM_VEL_INPUTS/2];
      
      // LowPass filter once more
      velocity_output = Filter::LowPass(velocity_output, median);

      // Set the return value
      *p = velocity_output;
      return p;
    }
};

#endif
