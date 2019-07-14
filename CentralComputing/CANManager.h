#ifndef CANMANAGER_H_
#define CANMANAGER_H_

#include "Defines.hpp"
#include "Command.h"
#include "SourceManagerBase.hpp"
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/uio.h>

const char hex_asc_upper[] = "0123456789ABCDEF";

#define hex_asc_upper_lo(x) hex_asc_upper[((x) & 0x0F)]
#define hex_asc_upper_hi(x) hex_asc_upper[((x) & 0xF0) >> 4]

static inline void put_hex_byte(char *buf, __u8 byte) {
  buf[0] = hex_asc_upper_hi(byte);
  buf[1] = hex_asc_upper_lo(byte);
}

class CANManager : public SourceManagerBase<CANData> {
 public:
  void set_relay_state(HV_Relay_Select relay, HV_Relay_State state);
  void set_motor_state(bool enable);
  void set_motor_throttle(int16_t value);

  // Public for testing purposes
  void initialize_sensor_error_configs();
  void check_for_sensor_error(const std::shared_ptr<CANData> &, E_States state);

 private:
  bool initialize_source();
  void stop_source();
  std::shared_ptr<CANData> refresh();
  std::shared_ptr<CANData> refresh_sim();

  CANData stored_data;

  // Heavily inspired by: https://github.com/linux-can/can-utils/blob/master/candump.c
  // https://www.can-cia.org/fileadmin/resources/documents/proceedings/2012_kleine-budde.pdf

  // Send data over CAN bus. len <= 8. Returns false if critical failure
  bool send_frame(uint32_t can_id, const char* buf, int len);

  // Non-blocking read of CAN bus. Reads one frame at a time, populates r_frame member variable. 
  // Returns false if critical failure
  bool recv_frame();

  uint32_t cast_to_u32(int offset, int bytes_per_item, uint8_t* bufferArray);
  void u32_to_bytes(uint32_t toCast, char* bufferArray);
  void u16_to_bytes(uint16_t toCast, char* bufferArray);
  void i16_to_bytes(int16_t toCast, char* bufferArray);

  // CAN socket
  int can_fd;
  // can_frame vs canfd_frame: https://computer-solutions.co.uk/info/Embedded_tutorials/can_tutorial.htm
  // Note: canfd_frame is newer, but backwards compatibble with can_frame

  // Used for CAN socket setup
  struct ifreq ifr;
  struct sockaddr_can addr;

  // Send Frame. Used when calling write(). Note: type can_frame, not canfd_frame
  struct can_frame s_frame;

  // Recv Frame. Used when calling recvmsg(). Note: type canfd_frame, not can_frame
  struct canfd_frame r_frame;
  struct msghdr msg;  // Used with recvmsg()
  struct iovec iov;   // Necessary part of msg, although we don't use it
  char ctrlmsg[CMSG_SPACE(1)];  // Necessary, although we dont use it. See: candump.c

  // CAN Frame IDs
  const unsigned int can_id_t1 = 385;      // 0x181
  const unsigned int can_id_t2 = 641;      // 0x281 
  const unsigned int can_id_t3 = 897;      // 0x381 
  const unsigned int can_id_p1 = 100;      // Get actual values
  const unsigned int can_id_p2 = 200;
  const unsigned int can_id_error = 129;   // 0x081
  const unsigned int can_id_bms_one = 54;  // 0x36
  const unsigned int can_id_bms_two = 53;
  const unsigned int can_id_bms_relay = 0x6A0;

  // uint32_t relay_state_buf;  // used while sending CAN Frames to BMS
  char relay_state_buf[3];
  // (reinterpret_cast<char*>(&relay_state_buf))[relay] = state;

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
  int32_t error_bms_logic_over_voltage; 
  int32_t error_bms_logic_under_voltage; 
  int32_t error_bms_internal_over_temp; 
  int32_t error_bms_rolling_counter_timeout;

  int32_t rolling_counter_tracker;
  int64_t rolling_counter_timer;

  std::mutex send_mutex;

  std::string name(){
    return "can";
  }
};
#endif  // CANMANAGER_H_
