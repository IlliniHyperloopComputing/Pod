#include "Brake.hpp"

Brake::Brake(){}

void Brake::enable_brakes() {
}

void Brake::disable_brakes() {
}

void Brake::set_enable(bool enable){

}

bool Brake::is_enabled(){
  return false;
}

int16_t Brake::calculate_brakeForce(double dt, int16_t last_brakeForce){
  return int16_t(0);
}

void Brake::set_brakeForce(int16_t value) {

}
