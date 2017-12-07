#ifndef SIMULATION_H 
#define SIMULATION_H
#include "Utils.h"
#include "unistd.h"
#include "stdint.h"

// Defines and utilities for simulation classes

enum Sim_Command {
  STATE = 0,
  MOTOR_ENABLE = 1,
  MOTOR_THROTTLE = 2,
  BRAKE_ENABLE = 3
  };

void write_sim_command(Sim_Command c, uint8_t value);
#endif //SIMULATION_H
