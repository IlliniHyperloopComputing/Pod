#include "active.h"

active::active(){
    printf("Entering active init\n");
    motor_levitation = new motor_control(BlackLib::pwmName::P9_22, BlackLib::gpioName::GPIO_45, BlackLib::gpioName::GPIO_69, BlackLib::gpioName::GPIO_66, 1000.0);

    motor_stability = new motor_control(BlackLib::pwmName::P8_13, BlackLib::gpioName::GPIO_68, BlackLib::gpioName::GPIO_49, BlackLib::gpioName::GPIO_117, 1000.0);
    brake = new brake_control(BlackLib::gpioName::GPIO_44, BlackLib::gpioName::GPIO_26);
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
