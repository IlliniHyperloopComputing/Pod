#ifndef PRU_MANAGER_HPP
#define PRU_MANAGER_HPP

#include "SourceManagerBase.hpp"
#include <sys/poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define DEVICE_NAME		"/dev/rpmsg_pru31"
#define MAX_BUFFER_SIZE		512

struct PRUData {
	uint32_t counts[11];
	uint32_t decays[11];
	uint32_t deltas[11];
};

class PRUManager : public SourceManagerBase<(long long) (0.010 * 1E6), PRUData> {
  private:
    bool initialize_source();
    std::shared_ptr<PRUData> refresh();

    uint8_t readBuf[MAX_BUFFER_SIZE];
	  struct pollfd pollfds[1];
      
};

#endif

