#ifdef SIM
#include "Motor.h"
#include "Simulation.h"

Motor::Motor() {

}


void Motor::enable_motors() {
  write_sim_command(MOTOR_ENABLE, 1);
}

void Motor::disable_motors() {
  write_sim_command(MOTOR_ENABLE, 2);

}

void Motor::set_throttle(double value) {
  uint8_t v = value * 255;
  write_sim_command(MOTOR_THROTTLE, v );

}

#endif
