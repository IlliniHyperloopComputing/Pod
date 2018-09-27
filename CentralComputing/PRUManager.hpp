#ifndef PRU_MANAGER_HPP
#define PRU_MANAGER_HPP

#include "SourceManagerBase.hpp"
#include "SourceManager.hpp"
#include <sys/poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define DEVICE_NAME		"/dev/rpmsg_pru31"
#define MAX_BUFFER_SIZE		512

#define CLOCK_TO_SEC (21.474836475/4294967295.0)

#define NUM_ENC_INPUTS 4
#define NUM_MOTOR_INPUTS 4

struct PRUData {
	double encoder_distance[NUM_ENC_INPUTS];
	double encoder_velocity[NUM_ENC_INPUTS];
  double disk_RPM[NUM_MOTOR_INPUTS];
};

struct RawPRUData {
	uint32_t counts[11];
	uint32_t decays[11];
	uint32_t deltas[11];
};

class PRUManager : public SourceManagerBase<(long long) (0.010 * 1E6), PRUData, true> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<PRUData> refresh();

    double convert_to_velocity(uint32_t decay, uint32_t delta, double distance);

    std::string name();

    uint8_t readBuf[MAX_BUFFER_SIZE];
	  struct pollfd pollfds[1];

    // Variables used for pru processing
    const int enc_idx[NUM_ENC_INPUTS] = {1, 2, 3, 4}; 
    const double enc_map[NUM_ENC_INPUTS] = {1, 1, 1, 1};
    const int disk_idx[NUM_MOTOR_INPUTS] = {5, 6, 7, 8}; 
    const double disk_map[NUM_MOTOR_INPUTS] = {1, 1, 1, 1};
      
};

#endif

