#ifndef CAN_MANAGER_HPP
#define CAN_MANAGER_HPP

#include "SourceManagerBase.hpp"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/uio.h>

struct CANData {
  //replace with actual data structure
  int dummy_data;
};

const char hex_asc_upper[] = "0123456789ABCDEF";

#define hex_asc_upper_lo(x) hex_asc_upper[((x) & 0x0F)]
#define hex_asc_upper_hi(x) hex_asc_upper[((x) & 0xF0) >> 4]

static inline void put_hex_byte(char *buf, __u8 byte)
{
  buf[0] = hex_asc_upper_hi(byte);
  buf[1] = hex_asc_upper_lo(byte);
}

class CANManager : public SourceManagerBase<(long long) (1.0 * 1E6), CANData, false> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<CANData> refresh();
    std::shared_ptr<CANData> refresh_sim();
    bool send_frame(uint32_t can_id, const uint8_t * buf, int len);
    bool recv_frame();

    // Heavily inspired by: https://github.com/linux-can/can-utils/blob/master/candump.c
    // https://www.can-cia.org/fileadmin/resources/documents/proceedings/2012_kleine-budde.pdf
    struct ifreq ifr;
    struct sockaddr_can addr;
    char ctrlmsg[CMSG_SPACE(sizeof(struct timeval) + 3*sizeof(struct timespec) + sizeof(__u32))];
    int can_fd;

    std::string name(){
      return "can";
    }

    //iterator for testing purposes, remove
    int i = 5;
};

#endif
