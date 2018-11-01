#include "Brakes.hpp"
using namespace Utils;

Brakes::Brakes(){

}

void Brakes::enable_brakes() {
    #ifdef SIM
    SimulatorManager::sim.sim_brake_enable();

    #endif
  print(LogLevel::LOG_DEBUG, "Brakes Enabled\n");
}

void Brakes::disable_brakes() {
    #ifdef SIM
    SimulatorManager::sim.sim_brake_disable();

    #endif

  print(LogLevel::LOG_DEBUG, "Brakes Disabled\n");
}

void Brakes::set_enable(bool enable){
}

bool Brakes::is_enabled(){
  return enabled;
}

int16_t Brakes::calculate_pressure(double dt, int16_t last_throttle){
    return (int16_t) 0;
}

void Brakes::set_pressure(int16_t value) {

    #ifdef SIM
    SimulatorManager::sim.sim_brake_set_pressure(value);

    #endif
      print(LogLevel::LOG_DEBUG, "Setting brake pressure: %d\n", value);
}
