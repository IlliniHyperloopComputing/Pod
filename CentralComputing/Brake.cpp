#include "Brake.hpp"
using namespace Utils;


Brake::Brake(){

}

void Brake::enable_Brakes() {

}

void Brake::disable_Brakes() {

}

void Brake::set_enable(bool enable){

}

bool Brake::is_enabled(){
  return enabled;
}

int16_t Brake::calculate_pressure(double dt, int16_t last_pressure){
  return 0;
}

void Brake::set_pressure(int16_t value) {

}
