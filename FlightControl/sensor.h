#ifndef SENSOR_STUFF
#define SENSOR_STUFF

#include <iostream>
#include <string>
#include <atomic>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

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
        std::atomic<double> *  get_atomic_temps();
        std::atomic<double> *  get_atomic_rpm();
    

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
        std::atomic<double> * atomic_temps;
        std::atomic<double> * atomic_rpm;
        std::atomic<uint8_t> x_status;
        std::atomic<uint8_t> z_status;
        std::atomic<uint8_t> lev_status;
        std::atomic<uint8_t> v_status;
        std::atomic<uint8_t> a_status;
        std::atomic<uint8_t> att_status;
        std::atomic<uint8_t> brake_pressure_status;

        int  i2c_thermo;
        int  i2c_rpm;
        int  open_i2c(int address);

        void init_x();
        void init_z();
        void init_lev();
        void init_v();
        void init_a();
        void init_att();
        void init_brake_pressure();
        void init_temps();
        void init_rpm();


        void update_x();
        void update_z();
        void update_lev();
        void update_v();
        void update_a();
        void update_att();
        void update_brake_pressure();
        //void update_esc();
        //void update_tot();
        void update_temp();
        void update_rpm();

        uint8_t tick;
};

#endif
