#include "MotorManager.hpp"
using namespace Utils;
void MotorManager::enable_motors() {
  integral = 0;
  print(LogLevel::LOG_DEBUG, "Enabling motors\n");
}

void MotorManager::disable_motors() {
  integral = 0;
  print(LogLevel::LOG_DEBUG, "Disabling motors\n");
}

uint16_t MotorManager::calculate_throttle(double dt, uint16_t last_throttle){
  // Get velocity
  // And current disk RPM
  std::shared_ptr<StateSpace> stateSpace = SourceManager::MM.Get();
  double velocity = stateSpace.get()->x[1];
  double rpm      = stateSpace.get()->rpm;

  double setPoint = velocity * MS_TO_RPM + RPM_PDR;

  double error = setPoint-rpm;
  integral += (error * dt);

  int output = (int) (RPM_TO_USEC * ((KP * error) + (KI * integral)));

  output = Utils::clamp(output, -2, 2);
  
  return (uint16_t)Utils::clamp(last_throttle + output, 0, 1000);

}

void MotorManager::set_throttle(uint16_t value) {
  print(LogLevel::LOG_DEBUG, "Setting motor throttle: %d\n", value);
}
