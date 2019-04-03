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
  uint32_t dummy_data;
  uint16_t status_word;
  int32_t position_val;
  int16_t torque_val;
  uint8_t controller_temp;
  uint8_t motor_temp;
  int32_t dc_link_voltage;
  int16_t logic_power_supply_voltage;
  int16_t current_demand;
  uint32_t motor_current_val;
  uint32_t electrical_angle;
  uint32_t phase_a_current;
  uint32_t phase_b_current; 
};

const char hex_asc_upper[] = "0123456789ABCDEF";

#define hex_asc_upper_lo(x) hex_asc_upper[((x) & 0x0F)]
#define hex_asc_upper_hi(x) hex_asc_upper[((x) & 0xF0) >> 4]

static inline void put_hex_byte(char *buf, __u8 byte)
{
  buf[0] = hex_asc_upper_hi(byte);
  buf[1] = hex_asc_upper_lo(byte);
}

class CANManager : public SourceManagerBase<CANData, false> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<CANData> refresh();
    std::shared_ptr<CANData> refresh_sim();

    // Send data over CAN bus. len <= 8. Returns false if critical failure
    bool send_frame(uint32_t can_id, const char* buf, int len);

    // Non-blocking read of CAN bus. Reads one frame at a time, populates r_frame member variable. Returns false if critical failure
    bool recv_frame();

    // Heavily inspired by: https://github.com/linux-can/can-utils/blob/master/candump.c
    // https://www.can-cia.org/fileadmin/resources/documents/proceedings/2012_kleine-budde.pdf

    // CAN socket
    int can_fd;

    // Used for CAN socket setup
    struct ifreq ifr;
    struct sockaddr_can addr;

    //can_frame vs canfd_frame: https://computer-solutions.co.uk/info/Embedded_tutorials/can_tutorial.htm
    //Note: canfd_frame is newer, but backwards compatibble with can_frame

    // Send Frame. Used when calling write(). Note: type can_frame, not canfd_frame
    struct can_frame s_frame;

    // Recv Frame. Used when calling recvmsg(). Note: type canfd_frame, not can_frame
    struct canfd_frame r_frame;
    struct msghdr msg; // Used with recvmsg()
    struct iovec iov;  // Necessary part of msg, although we don't use it
    char ctrlmsg[CMSG_SPACE(1)]; // Necessary, although we dont use it. See: candump.c

    std::string name(){
      return "can";
    }
};

#endif
