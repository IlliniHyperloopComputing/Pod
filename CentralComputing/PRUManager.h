#ifndef PRUMANAGER_H_
#define PRUMANAGER_H_

#include "SourceManagerBase.hpp"
#include "SourceManager.h"
#include "Defines.hpp"
#include <sys/poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

//#define DEVICE_NAME   "/dev/rpmsg_pru31"
#define MAX_BUFFER_SIZE   512


//#define CLOCK_TO_SEC ((double)21.474836475 / (double)4294967295.0)
/**
struct RawPRUData {
  uint32_t counts[11];
  uint32_t decays[11];
  uint32_t deltas[11];
};
**/

class PRUManager : public SourceManagerBase<PRUData> {
 private:
    const int32_t WHEEL_CIRCUMFERENCE_IN_MM = 500;
    const int32_t HUNDRED_FEET_IN_MM = 30480;
    const int WHEEL_GPIO_ONE = 66;
    const int WHEEL_GPIO_TWO = 26;
    const int ORANGE_TAPE_GPIO_ONE = 46;
    const int ORANGE_TAPE_GPIO_TWO = 65;
    const int WATCHDOG_GPIO =111;
    int64_t wheel_one_last_time = Utils::microseconds();
    int64_t wheel_two_last_time = Utils::microseconds();
    int64_t orange_one_last_time = Utils::microseconds();
    int64_t orange_two_last_time = Utils::microseconds();
    int64_t watchdog_last_time = Utils::microseconds();

    bool initialize_source();
    void stop_source();
    std::shared_ptr<PRUData> refresh();
    std::shared_ptr<PRUData> refresh_sim();

    //int32_t convert_to_velocity(uint32_t decay, uint32_t delta, uint32_t distance);
    int check_GPIO(int GPIONumber);
    std::string name();

    uint8_t readBuf[MAX_BUFFER_SIZE];
    //struct pollfd pollfds[1];

    // Variables used for pru processing
    //const int orange_idx[NUM_ORANGE_INPUTS] = {4, 5}; 
    //const uint32_t orange_map[NUM_ORANGE_INPUTS] = {HUNDRED_FEET_IN_MM, HUNDRED_FEET_IN_MM};
    //const int wheel_idx[NUM_WHEEL_INPUTS] = {0, 1}; //p8_45 and p8_46
    //const uint32_t wheel_map[NUM_WHEEL_INPUTS] = {WHEEL_CIRCUMFRENCE_IN_MM, WHEEL_CIRCUMFRENCE_IN_MM};

    int32_t error_orange_diff;
    int32_t error_orange_diff_count;
    int32_t error_encoder_wheel_diff;
    int32_t error_encoder_wheel_diff_count;
    int32_t error_watchdog_heartbeat_min_hz;


    int32_t orange_diff_counter;
    int32_t wheel_diff_counter;

    //bool do_reset = 0;
    //int64_t reset_timeout_start;
    //std::mutex reset_mutex;

 public:

   //void reset_pru();
    // Public for testing purposes
    void initialize_sensor_error_configs();
    void check_for_sensor_error(const std::shared_ptr<PRUData> &, E_States state);
};

#endif  // PRUMANAGER_H_
