//
// Created by Leo Yang on 2019/11/7.
//

#ifndef COMMAND_H_
#define COMMAND_H_

#include "Utils.h"
#include "SafeQueue.hpp"
#include "Event.h"
#include "Defines.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>
#include <atomic>
#include <thread> // NOLINT
#include <memory>
#include <sys/ioctl.h>

namespace Command {

extern SafeQueue<uint64_t> command_queue;
struct Network_Command;
void put(uint32_t id, uint32_t value);
bool get(Network_Command * com);
void wait_for_empty();
void flush();

enum Network_Command_ID {
  // state transitions
      TRANS_SAFE_MODE = 0,
  TRANS_FUNCTIONAL_TEST_OUTSIDE = 1,
  TRANS_LOADING = 2,
  TRANS_FUNCTIONAL_TEST_INSIDE = 3,
  TRANS_LAUNCH_READY = 4,
  TRANS_FLIGHT_ACCEL = 5,
  ENABLE_MOTOR = 6,
  DISABLE_MOTOR = 7,
  SET_MOTOR_SPEED = 8,
  ENABLE_BRAKE = 9,
  DISABLE_BRAKE = 10,
  TRANS_FLIGHT_COAST = 11,
  TRANS_FLIGHT_BRAKE = 12,
  TRANS_ABORT = 13,    // Move to either safe mode, or flight abort
  SET_ADC_ERROR = 14,  // SET_XXX_ERROR defines must be one after another, in one block
  SET_CAN_ERROR = 15,
  SET_I2C_ERROR = 16,
  SET_PRU_ERROR = 17,
  SET_NETWORK_ERROR = 18,
  SET_OTHER_ERROR = 19,
  CLR_ADC_ERROR = 20,  // CLR_XXX_ERROR defines must be one after another, in one block
  CLR_CAN_ERROR = 21,
  CLR_I2C_ERROR = 22,
  CLR_PRU_ERROR = 23,
  CLR_NETWORK_ERROR = 24,
  CLR_OTHER_ERROR = 25,
  SET_HV_RELAY_HV_POLE = 26,
  SET_HV_RELAY_LV_POLE = 27,
  SET_HV_RELAY_PRE_CHARGE = 28,
  CALC_ACCEL_ZERO_G = 29,
  RESET_PRU = 30,
  SENTINEL = 31,  // Any Command above this value is an invalid command
  // MUST ADD THESE TO TRANSITION LIST
  // Update the get_string() function bellow with additional commands, or suffer segfaults
};

// enum specifying what data is sent
// [1 byte Data ID][4 byte size][size byte chunk]
enum Network_Data_ID {
  POD_STATE,
  BRAKE_STATUS,
  MOTOR_STATUS,
  POSITION,
  VELOCITY,
  ACCELERATION,
  TEMPERATURE
};

struct Network_Command {
  // state transtitions
  uint32_t id;     // id is a Network_Command_ID
  uint32_t value;  // value of a command
};

// returns the string name for command value
std::string get_network_command_ID_string(uint command);

// Parses the value of a command, returns string
std::string get_network_command_value_string(Network_Command * com);

// To make error flag setting easy, this helper functionn is defined
// Why use this instead of just calling Command::put() ?
// This function is "safe" and will not spam the queue.
// Say something is faulting every cycle. Initially it will set the error flag
// But then it send another command to the Unified Queue once every second.
// This helper function, and the int64_t array of timers makes that possible
#define FLAGS_PER_ERROR 32
extern int64_t error_flag_timers[FLAGS_PER_ERROR * 6];  // 32 flags per error ID, 6 errors
extern std::mutex error_flag_mutex;
void set_error_flag(Network_Command_ID id, uint32_t value);

}  // namespace Command

#endif  // COMMAND_H_

