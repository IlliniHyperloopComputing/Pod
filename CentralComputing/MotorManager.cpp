#include "MotorManager.hpp"
using namespace Utils;

void MotorManager::enable_motors() {
  integral = 0;
  enabled = true;
  set_throttle(MOTOR_OFF);
  print(LogLevel::LOG_DEBUG, "Motors Enabled\n");
}

void MotorManager::disable_motors() {
  set_throttle(MOTOR_OFF);
  integral = 0;
  enabled = false;

  print(LogLevel::LOG_DEBUG, "Motors Disabled\n");
}

bool MotorManager::is_enabled(){
  return enabled;
}

int16_t MotorManager::calculate_throttle(double dt, int16_t last_throttle){
  // Get velocity
  // And current disk RPM
  std::shared_ptr<StateSpace> stateSpace = SourceManager::MM.Get();
  double velocity = stateSpace.get()->x[1];
  double rpm      = stateSpace.get()->rpm;

  // Calculate setpoint
  double setPoint = velocity * MS_TO_RPM + RPM_PDR;

  // Calculate error
  double error = setPoint-rpm;
  // Calculate integral term
  integral += (error * dt);


  //TODO: Set limit on dt ?
  //TODO: Clamp integral term ? 

  // Calculate PI output
  int output = (int) (RPM_TO_USEC * ((KP * error) + (KI * integral)));

  // Clamp output to around the last_throttle
  output = last_throttle + Utils::clamp(output - last_throttle, -2, 2);
  
  return (int16_t) Utils::clamp(output, 0, 1000);
}

void MotorManager::set_throttle(int16_t value) {
  if(enabled){
    print(LogLevel::LOG_DEBUG, "Setting motor throttle: %d\n", value);
  }
}
