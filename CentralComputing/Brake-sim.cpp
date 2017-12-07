#ifdef SIM
#include "Brake.h"
#include "Simulation.h"

Brake::Brake(Xmega * xmega) {

}

void Brake::enable_brakes() {
  write_sim_command(BRAKE_ENABLE, 1);
}

void Brake::disable_brakes() {
  write_sim_command(BRAKE_ENABLE, 0);
}

#endif
