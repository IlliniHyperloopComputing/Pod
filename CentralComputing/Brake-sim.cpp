#ifdef SIM
#include "Brake.h"

Brake::Brake(Xmega * xmega) {

}

void Brake::enable_brakes() {

}

void Brake::disable_brakes() {

}

void Brake::set_brake_value(double value) {
  //override point, sends desired braking force to Simulator
}
#endif
