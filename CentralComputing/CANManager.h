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
  uint16_t status_word;
  int32_t position_val;
  int16_t torque_val;
  uint8_t controller_temp;
  uint8_t motor_temp;
  int32_t dc_link_voltage;
  int16_t logic_power_supply_voltage;
  int16_t current_demand;
  uint8_t motor_current_val;
  int16_t electrical_angle;
  int16_t phase_a_current;
  int16_t phase_b_current;
  int relay_state;
  int rolling_counter;
  int fail_safe_sate;
  int peak_current;
  int pack_voltage_inst;
  int pack_voltage_open;
  int pack_sol;
  int pack_amphours;
  int pack_resistance;
  int pack_dod;
  int pack_soh;
  int current_limit_status;
  int max_pack_dcl;
  int avg_pack_current;
  int highest_temp;
  int highest_temp_id;
  int avg_temp;
  int internal_temp;
  int low_cell_voltge;
  int low_cell_voltage_id; 
  int high_cell_voltage;
  int high_cell_voltage_id;
  int low_cell_internalR;
  int low_cell_internalR_id;
  int high_cell_internalR;
  int high_cell_internalR_id;
  int power_voltage_input;
  int dtc_status_one;
  int dtc_status_two;
  int adaptive_total_cap;
  int adaptive_amphours;
  int adaptive_soc;
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

    uint32_t cast_to_u32(int offset, int bytes_per_item, unsigned char* bufferArray);
    void u32_to_bytes(uint32_t toCast, unsigned char* bufferArray);
    void u16_to_bytes(uint16_t toCat, unsigned char* bufferArray);
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
