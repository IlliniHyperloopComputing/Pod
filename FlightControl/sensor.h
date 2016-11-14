#ifndef SENSOR_STUFF
#define SENSOR_STUFF

#include <iostream>
#include <string>
#include <atomic>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <map>
#include <sys/ioctl.h>

class sensor{
    public:
        sensor();
        ~sensor();
        void update();
        std::atomic<double> *  get_atomic_x();
        std::atomic<double> *  get_atomic_z();
        std::atomic<double> *  get_atomic_lev();
        std::atomic<double> *  get_atomic_v();
        std::atomic<double> *  get_atomic_a();
        std::atomic<double> *  get_atomic_att();
        std::atomic<double> *  get_atomic_brake();
        std::atomic<double> *  get_atomic_temps();
        std::atomic<double> *  get_atomic_rpm();
        std::atomic<double> *  get_atomic_tape_count();
    

    private:

        std::atomic<double>  atomic_x;
        std::atomic<double>  atomic_z;
        std::atomic<double> * atomic_lev;
        std::atomic<double> * atomic_v;
        std::atomic<double> * atomic_a;
        std::atomic<double> * atomic_att;
        std::atomic<double>  atomic_brake_pressure;
        std::atomic<double> * atomic_temps;
        std::atomic<double> * atomic_rpm;
        std::atomic<double> * atomic_tape_count;

		std::map<double, int> count_to_distance;

        int  i2c_thermo;
        int  i2c_rpm;
		int  i2c_tape;
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
        void init_tape_count();
		void init_map();

        void update_x();
        void update_z();
        void update_lev();
        void update_v();
        void update_a();
        void update_att();
        void update_brake_pressure();
        void update_temp();
        void update_rpm();
		void update_tape_count();

        uint8_t tick;
};

#endif
