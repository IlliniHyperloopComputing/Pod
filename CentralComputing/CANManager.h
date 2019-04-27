#ifndef CAN_MANAGER_HPP
#define CAN_MANAGER_HPP

#include "Defines.hpp"
#include "SourceManagerBase.hpp"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/uio.h>


const char hex_asc_upper[] = "0123456789ABCDEF";

#define hex_asc_upper_lo(x) hex_asc_upper[((x) & 0x0F)]
#define hex_asc_upper_hi(x) hex_asc_upper[((x) & 0xF0) >> 4]

static inline void put_hex_byte(char *buf, __u8 byte)
{
  buf[0] = hex_asc_upper_hi(byte);
  buf[1] = hex_asc_upper_lo(byte);
}

class CANManager : public SourceManagerBase<CANData> {
  private:
    bool initialize_source();
    void stop_source();
    std::shared_ptr<CANData> refresh();
    std::shared_ptr<CANData> refresh_sim();

    // Send data over CAN bus. len <= 8. Returns false if critical failure
    bool send_frame(uint32_t can_id, const char* buf, int len);

    // Non-blocking read of CAN bus. Reads one frame at a time, populates r_frame member variable. Returns false if critical failure
    bool recv_frame();

    uint32_t cast_to_u32(int offset, int bytes_per_item, unsigned char* bufferArray);
    void u32_to_bytes(uint32_t toCast, unsigned char* bufferArray);
    void u16_to_bytes(uint16_t toCat, unsigned char* bufferArray);
    void initialize_sensor_error_configs();
    void check_for_sensor_error(const std::shared_ptr<CANData> &);
    // Heavily inspired by: https://github.com/linux-can/can-utils/blob/master/candump.c
    // https://www.can-cia.org/fileadmin/resources/documents/proceedings/2012_kleine-budde.pdf

    // CAN socket
    int can_fd;
    const unsigned int can_id_t1 = 385; //0x181
    const unsigned int can_id_t2 = 641; //0x281 
    const unsigned int can_id_t3 = 897; //0x381 
    const unsigned int can_id_p1 = 100; //Get actual values
    const unsigned int can_id_p2 = 200;
    const unsigned int can_id_error = 129; //0x081
    const unsigned int can_id_bms_one = 54; //0x36
    const unsigned int can_id_bms_two = 53;
    // Used for CAN socket setup
    struct ifreq ifr;
    struct sockaddr_can addr;

    int32_t error_motor_ctrl_over_temp;
    int32_t error_motor_over_temp;
    int32_t error_dc_link_over_voltage; 
    int32_t error_dc_link_under_voltage; 
    int32_t error_motor_ctrl_logic_over_voltage; 
    int32_t error_motor_ctrl_logic_under_voltage; 

    int32_t error_cell_over_voltage;
    int32_t error_cell_under_voltage;
    int32_t error_cell_over_temp;
    int32_t error_battery_over_voltage;
    int32_t error_battery_under_voltage;
    int32_t error_battery_over_current;
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
#endif
