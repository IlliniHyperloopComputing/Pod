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
#include <sys/time.h>
#include <chrono>
#include <boost/format.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "ADS1115.h"
#include "status.h"


class sensor{
	typedef std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<long long int, std::ratio<1ll, 1000000000ll> > > time_t; 
    public:
        sensor(std::vector<status_message_ptr> * tmp_status_buff);
        ~sensor();
        void update();
        std::atomic<double> *  get_atomic_z();
        std::atomic<double> *  get_atomic_lev();
        std::atomic<double> *  get_atomic_v();
        std::atomic<double> *  get_atomic_a();
        std::atomic<double> *  get_atomic_att();
        std::atomic<double> *  get_atomic_brake();
        std::atomic<double> *  get_atomic_temps();
        std::atomic<double> *  get_atomic_rpm();
        std::atomic<double> *  get_atomic_tape_count();
		std::atomic<double> *  get_distance();
    

    private:

        std::atomic<double>  atomic_z;
        std::atomic<double> * atomic_lev;
        std::atomic<double> * atomic_v;
        std::atomic<double> * atomic_a;
        std::atomic<double> * atomic_att;
        std::atomic<double>  atomic_brake_pressure;
        std::atomic<double> * atomic_temps;
        std::atomic<double> * atomic_rpm;
        std::atomic<double>  atomic_tape_count;
		std::atomic<double>  distance;

		bool remain_1000;
		bool remain_500;
		time_t  last_times[4];
		double distance_at_1000;

        int  i2c_brake;
        ADS1115* i2c_brake_adc; 
        int  i2c_thermo;
        int  i2c_rpm;
		int  i2c_tape;
		int  i2c_a;
        ADS1115* i2c_a_adc; 
        int  open_i2c(int address);

        int  init_z();
        int  init_lev();
        int  init_v();
        int  init_a();
        int  init_att();
        int  init_brake_pressure();
        int  init_temps();
        int  init_rpm();
        int  init_tape_count();
        std::vector<status_message_ptr> * tmp_status_buff;

        void update_z();
        void update_lev();
        void update_v();
        void update_a();
        void update_att();
        void update_brake_pressure();
        void update_temp();
        void update_rpm();
		void update_tape_count();

		void reset_tape_count();
        uint8_t tick;
};

#endif
