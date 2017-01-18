#include "active.h"

active::active(){
    printf("Entering active init\n");
    motor_levitation = new motor_control(BlackLib::pwmName::P9_16, BlackLib::gpioName::GPIO_60, 1000.0);
    motor_stability = new motor_control(BlackLib::pwmName::P9_22, BlackLib::gpioName::GPIO_39, 1000.0);
    brake = new brake_control(BlackLib::gpioName::GPIO_46, BlackLib::gpioName::GPIO_65);
    printf("Exiting active init\n");

}

active::~active(){
    if(motor_levitation != NULL)
        delete motor_levitation;
    if(motor_stability != NULL)
        delete motor_stability;
    if(brake != NULL)
        delete brake;
}


void active::set_lev(double microseconds){
    motor_levitation->set_microseconds(microseconds); 
}

void active::set_sta(double microseconds){
    motor_stability->set_microseconds(microseconds);
}

void active::low_lev(){
    motor_levitation->set_low();
}
void active::low_sta(){
    motor_stability->set_low();
}

void active::on_lev(){
    motor_levitation->on();
}

void active::on_sta(){
    motor_stability->on();
}

void active::off_lev(){
    motor_levitation->off();
}

void active::off_sta(){
    motor_stability->off(); 
}


void active::forward_brake(){
    brake->forward();
}

void active::backward_brake(){
    brake->backward();
}

void active::stop_brake(){
    brake->stop();
}
