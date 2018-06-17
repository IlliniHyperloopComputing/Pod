#ifndef SIM
#include "Motor.h"

#define STOP 0
#define RUN  1

const std::string Motor::runPath = "/path/path";
const std::string Motor::dutyPath = "/path/path";
const std::string Motor::periodPath = "/path/path";


Motor::Motor() {
  //need to set period length
  set_period(20000000);
}


void Motor::enable_motors() {
  //need to set duty cycle to 100%
  //need to set pwm on
  //
  std::ofstream fl;
  fl.open(this->runPath.c_str(), std::ios::out);
  if(fl.fail()){
    fl.close();
    print_info("FAILURE TO ENABLE MOTOR PWM");
  }
  else{
    fl << RUN;
    fl.close();
  }
}

void Motor::disable_motors() {
  //need to set pwm off
  //need to set pwm to 0% duty
  std::ofstream fl;
  fl.open(this->runPath.c_str(), std::ios::out);
  if(fl.fail()){
    fl.close();
    print_info("FAILURE TO ENABLE MOTOR PWM");
  }
  else{
    fl << STOP;
    fl.close();
  }
}

bool Motor::set_throttle(double value) {
  //need to set throttle here

  return true;
}

//in nano seconds
//So we need to set between 1000 and 2000 microseconds
//900 microseconds to arm
bool Motor::set_duty(uint64_t period){
  std::ofstream fl;
  fl.open(this->dutyPath.c_str(), std::ios::out);
  if(fl.fail()){
    fl.close();
    print_info("FAILURE TO SET PWM DUTY");
    return false;
  }
  else{
    fl << period;
    fl.close();
    return true;
  }
}

//in nano seconds
//for our application, we want a 20 millisecond period
//which is 20000000 nanoseconds
bool Motor::set_period(uint64_t period){
  std::ofstream fl;
  fl.open(this->periodPath.c_str(), std::ios::out);
  if(fl.fail()){
    fl.close();
    print_info("FAILURE TO SET PWM PERIOD");
    return false;
  }
  else{
    fl << period;
    fl.close();
    return true;
  }
   
}

#endif
