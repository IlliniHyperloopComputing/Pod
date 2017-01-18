#include <iostream>
#include "../BlackLib/v3_0/BlackGPIO/BlackGPIO.h"
#include "motors.h"
#include "brakes.h"

class active{
    public:
        active();
        ~active();
        
        void set_lev(double microseconds);
        void set_sta(double microseconds);

        void low_lev();
        void low_sta();
        
        void on_lev();
        void on_sta();
        void off_lev();
        void off_sta();

        void forward_brake();
        void backward_brake();
        void stop_brake();

    private:
        motor_control *motor_levitation;
        motor_control *motor_stability;
        brake_control *brake;

};
