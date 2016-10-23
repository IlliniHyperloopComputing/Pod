#ifndef SENSOR_STUFF
#define SENSOR_STUFF

#include <iostream>
#include <string>
#include <atomic>

class sensor{
    public:
        sensor();
        ~sensor();
        void update();
        //void create_message(std::string & buff);
    
        /*double * get_x();
        double * get_z();
        double * get_lev();
        double * get_v();
        double * get_a();
        double * get_att();
        double * get_brake();
        double * get_esc();
        double * get_tot();
        */

        std::atomic<double> *  get_atomic_x();
        std::atomic<double> *  get_atomic_z();
        std::atomic<double> *  get_atomic_lev();
        std::atomic<double> *  get_atomic_v();
        std::atomic<double> *  get_atomic_a();
        std::atomic<double> *  get_atomic_att();
        std::atomic<double> *  get_atomic_brake();
        std::atomic<double> *  get_atomic_esc();
        std::atomic<double> *  get_atomic_tot();


    private:
        /*double x, z;
        double * lev;           //levf, levb
        double * v;             //vx, vy, vz;
        double * a;             //ax, ay, az;
        double * att;             //ax, ay, az;
        double brake_pressure;  //
        double * esc;           //fl, fr, bl, br; 
        double * tot;           //cu, am, b1, b2;
        */

        std::atomic<double>  atomic_x;
        std::atomic<double>  atomic_z;
        std::atomic<double> * atomic_lev;
        std::atomic<double> * atomic_v;
        std::atomic<double> * atomic_a;
        std::atomic<double> * atomic_att;
        std::atomic<double>  atomic_brake_pressure;
        std::atomic<double> * atomic_esc;
        std::atomic<double> * atomic_tot;
        

        void update_x();
        void update_z();
        void update_lev();
        void update_v();
        void update_a();
        void update_att();
        void update_brake_pressure();
        void update_esc();
        void update_tot();

        uint8_t tick;
};

#endif
