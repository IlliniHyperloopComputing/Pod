#ifndef PRUMANAGER_H_
#define PRUMANAGER_H_

#include "SourceManagerBase.hpp"
#include "SourceManager.h"
#include "Defines.hpp"
#include <sys/poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define DEVICE_NAME   "/dev/rpmsg_pru31"
#define MAX_BUFFER_SIZE   512

#define HUNDRED_FEET_IN_MM (30480)
#define WHEEL_CIRCUMFRENCE_IN_MM (500)

#define CLOCK_TO_SEC (21.474836475/4294967295.0)

struct RawPRUData {
  uint32_t counts[11];
  uint32_t decays[11];
  uint32_t deltas[11];
};

class PRUManager : public SourceManagerBase<PRUData> {
 private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<PRUData> refresh();
    std::shared_ptr<PRUData> refresh_sim();
    void initialize_sensor_error_configs();
    void check_for_sensor_error(const std::shared_ptr<PRUData> &);

    int32_t convert_to_velocity(uint32_t decay, uint32_t delta, uint32_t distance);

    std::string name();

    uint8_t readBuf[MAX_BUFFER_SIZE];
    struct pollfd pollfds[1];

    // Variables used for pru processing
    const int orange_idx[NUM_ORANGE_INPUTS] = {1, 2}; 
    const uint32_t orange_map[NUM_ORANGE_INPUTS] = {WHEEL_CIRCUMFRENCE_IN_MM, WHEEL_CIRCUMFRENCE_IN_MM};
    const int wheel_idx[NUM_WHEEL_INPUTS] = {3, 4}; 
    const uint32_t wheel_map[NUM_WHEEL_INPUTS] = {HUNDRED_FEET_IN_MM, HUNDRED_FEET_IN_MM};

    int32_t error_orange_diff;
    int32_t error_encoder_wheel_diff;
};

#endif  // PRUMANAGER_H_
