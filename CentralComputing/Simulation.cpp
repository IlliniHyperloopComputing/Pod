#include "Simulation.h"

void write_sim_command(Sim_Command c, uint8_t value) {
#ifdef SIM
  write(STDOUT_FILENO, &c, 1);
  write(STDOUT_FILENO, &value, 1);
 
#endif
}
