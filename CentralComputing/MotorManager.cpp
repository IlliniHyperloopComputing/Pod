#include "MotorManager.hpp"
using namespace Utils;

const std::string MotorManager::pwm_chip = "/sys/class/pwm/pwmchip0/";
const std::string MotorManager::pwm_pin =  "/sys/class/pwm/pwmchip0/pwm0/";

MotorManager::MotorManager(){
  // Following guide in: 
  // https://www.teachmemicro.com/beaglebone-black-pwm-ubuntu-device-tree/
  
  // This is for EHRPWM1A , P9_14
  // I use system() because I'm lazy

  std::ifstream in(pwm_chip + "export");
  if(in){
    system(("echo 0 > " + pwm_chip + "export").c_str() );
    system(("echo 0 > " + pwm_pin + "polarity").c_str() );
    system(("echo 20000000 > " + pwm_pin + "period").c_str() );
  }
  else{
    print(LogLevel::LOG_ERROR, "Motor Manager setup failed. Can't open\n");
  }

  in.close();

}

void MotorManager::enable_motors() {
  integral = 0;
  enabled = true;
  set_throttle(MOTOR_OFF);
  set_enable(true);

  print(LogLevel::LOG_DEBUG, "Motors Enabled\n");
}

void MotorManager::disable_motors() {
  set_throttle(MOTOR_OFF);
  set_enable(false);
  integral = 0;
  enabled = false;

  print(LogLevel::LOG_DEBUG, "Motors Disabled\n");
}

void MotorManager::set_enable(bool enable){

  std::ofstream out(pwm_chip + "enable", std::ofstream::trunc);
  if(out){
    if(enable){
      out << "1";
    }
    else{
      out << "0";
    }
  }
  out.close();

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
    std::ofstream out(pwm_chip + "duty_cycle", std::ofstream::trunc);
    if(out){
      // Why do we need conversion?
      // throttle setting is from 0 to 1000 (negative values are off)
      // because that is easy to understand
      // need to add 1000 to get into 1000 to 2000 range
      // need to be in nanoseconds, so multiply by 1000
      long long nano_sec = (value + 1000) * 1000;
      out << nano_sec;
    }
    out.close();

    print(LogLevel::LOG_DEBUG, "Setting motor throttle: %d\n", value);
  }
}
