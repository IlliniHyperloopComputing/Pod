#ifndef COMMAND_H_
#define COMMAND_H_

#include "Utils.h"
#include "SafeQueue.hpp"
#include "Event.h"
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
  SENTINEL = 30,  // Any Command above this value is an invalid command
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

// returns the string name for current state
std::string get_network_command_ID_string(uint command);
std::string get_network_command_ID_string(uint command) {
  if (command >= SENTINEL) {
    command = SENTINEL;
  }
  std::string commands[] = {
    "TRANS_SAFE_MODE",
    "TRANS_FUNCTIONAL_TEST_OUTSIDE",
    "TRANS_LOADING",
    "TRANS_FUNCTIONAL_TEST_INSIDE",
    "TRANS_LAUNCH_READY",
    "TRANS_FLIGHT_ACCEL",
    "ENABLE_MOTOR",
    "DISABLE_MOTOR",
    "SET_MOTOR_SPEED",
    "ENABLE_BRAKE",
    "DISABLE_BRAKE",
    "TRANS_FLIGHT_COAST",
    "TRANS_FLIGHT_BRAKE",
    "TRANS_ABORT",
    "SET_ADC_ERROR",
    "SET_CAN_ERROR",
    "SET_I2C_ERROR",
    "SET_PRU_ERROR",
    "SET_NETWORK_ERROR",
    "SET_OTHER_ERROR",
    "CLR_ADC_ERROR",
    "CLR_CAN_ERROR",
    "CLR_I2C_ERROR",
    "CLR_PRU_ERROR",
    "CLR_NETWORK_ERROR",
    "CLR_OTHER_ERROR",
    "SET_HV_RELAY_HV_POLE",
    "SET_HV_RELAY_LV_POLE",
    "SET_HV_RELAY_PRE_CHARGE",
    "CALC_ACCEL_ZERO_G",
    "SENTINEL -- INVALID COMMAND",
  };
  return commands[command];
}

std::string get_network_command_value_string(Network_Command * com);
std::string get_network_command_value_string(Network_Command * com) {
  if (com->id == SET_ADC_ERROR || com->id == CLR_ADC_ERROR) {
    if (com->value >= ADCErrors::ADC_SENTINEL || com->value == 0) {
      return "INVALID";
    }
    std::string commands[] = {
      "ADC_SETUP_FAILURE",
      "ADC_READ_ERROR",
      "ADC_ACCEL_DIFF_ERROR",
      "ADC_PNEUMATIC_OVER_PRESSURE_ERROR_1",
      "ADC_PNEUMATIC_OVER_PRESSURE_ERROR_2",
      "ADC_PNEUMATIC_OVER_PRESSURE_ERROR_3",
      "ADC_PNEUMATIC_OVER_PRESSURE_ERROR_4",
      "ADC_BATTERY_BOX_OVER_PRESSURE_ERROR",
      "ADC_BATTERY_BOX_UNDER_PRESSURE_ERROR",
    };
    int val = std::log2(com->value);
    return commands[val];
  } else if (com->id == SET_CAN_ERROR || com->id == CLR_CAN_ERROR) {
    if (com->value >= CANErrors::CAN_SENTINEL || com->value == 0) {
      return "INVALID";
    }
    std::string commands[] = {
      "CAN_SETUP_FAILURE",
      "CAN_SEND_FRAME_ERROR",
      "CAN_RECV_FRAME_ERROR",
      "CAN_MOTOR_CONTROLLER_HV_OVER_VOLTAGE_ERROR",
      "CAN_MOTOR_CONTROLLER_HV_UNDER_VOLTAGE_ERROR",
      "CAN_MOTOR_CONTROLLER_OVER_CURRENT",
      "CAN_MOTOR_CONTROLLER_MOTOR_OVER_TEMPERATURE",
      "CAN_MOTOR_CONTROLLER_INTERNAL_ERROR",
      "CAN_MOTOR_CONTROLLER_LV_OVER_VOLTAGE_ERROR",
      "CAN_MOTOR_CONTROLLER_LV_UNDER_VOLTAGE_ERROR",
      "CAN_MOTOR_CONTROLLER_INTERNAL_OVER_TEMPERATURE",
      "CAN_BMS_CELL_OVER_VOLTAGE",
      "CAN_BMS_CELL_UNDER_VOLTAGE",
      "CAN_BMS_CELL_OVER_TEMP",
      "CAN_BMS_BATTERY_OVER_VOLTAGE",
      "CAN_BMS_BATTERY_UNDER_VOLTAGE",
      "CAN_BMS_BATTERY_OVER_CURRENT",
      "CAN_SENTINEL",
    };
    int val = std::log2(com->value);
    return commands[val];
  } else if (com->id == SET_I2C_ERROR || com->id == CLR_I2C_ERROR) {
    if (com->value >= I2CErrors::I2C_SENTINEL || com->value == 0) {
      return "INVALID";
    }
    std::string commands[] = {
      "I2C_SETUP_FAILURE",
      "I2C_WRITE_ERROR",
      "I2C_READ_ERROR",
      "I2C_OVER_TEMP_ONE",
      "I2C_OVER_TEMP_TWO",
      "I2C_OVER_TEMP_THREE",
      "I2C_SENTINEL",
    };
    int val = std::log2(com->value);
    return commands[val];
  } else if (com->id == SET_PRU_ERROR || com->id == CLR_PRU_ERROR) {
    if (com->value >= PRUErrors::PRU_SENTINEL || com->value == 0) {
      return "INVALID";
    }
    std::string commands[] = {
      "PRU_SETUP_FAILURE",
      "PRU_WRITE_ERROR",
      "PRU_READ_ERROR",
      "PRU_ORANGE_DIFF_ERROR",
      "PRU_WHEEL_DIFF_ERROR",
      "PRU_SENTINEL",
    };
    int val = std::log2(com->value);
    return commands[val];
  } else if (com->id == SET_NETWORK_ERROR || com->id == CLR_NETWORK_ERROR) {
    if (com->value >= NETWORKErrors::NET_SENTINEL || com->value == 0) {
      return "INVALID";
    }
    std::string commands[] = {
      "UDP_SETUP_FAILURE",
      "TCP_SETUP_FAILURE",
      "UDP_READ_WRITE_ERROR",
      "TCP_READ_WRITE_ERROR",
      "UDP_DISCONNECT_ERROR",
      "TCP_DISCONNECT_ERROR",
      "NET_SENTINEL",
    };
    int val = std::log2(com->value);
    return commands[val];
  } else if (com->id == SET_OTHER_ERROR || com->id == CLR_OTHER_ERROR) {
    if (com->value >= OTHERErrors::GPIO_SENTINEL || com->value == 0) {
      return "INVALID";
    }
    std::string commands[] = {
      "GPIO_SWITCH_ERROR",
      "GPIO_SENTINEL",
    };
    int val = std::log2(com->value);
    return commands[val];
  } else if (com->id == SET_MOTOR_SPEED) {
    // Motor Speed
    char buff[100];
    snprintf(buff, sizeof(buff), "Set Throttle: %d/1000", com->value);
    return std::string(buff);
  } else if (com->id >= SET_HV_RELAY_HV_POLE  && com->id <= SET_HV_RELAY_PRE_CHARGE) {
    // Relays
    return com->value?"Relay On":"Relay Off";
  } else {
    return "N/A";
  }

}


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
