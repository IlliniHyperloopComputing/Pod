#include "Command.h"

SafeQueue<uint64_t> Command::command_queue;
int64_t Command::error_flag_timers[FLAGS_PER_ERROR * 6]; 

void Command::put(uint32_t id, uint32_t value) {
  uint64_t toQueue = (uint64_t)(((uint64_t)id) << 32) | (uint64_t)(value & 0xFFFFFFFF);
  command_queue.enqueue(toQueue);
}

bool Command::get(Network_Command * com) {
  uint64_t preShift;
  if (!command_queue.dequeue(&preShift)) {
    return false;
  }
  com->value = static_cast<uint32_t>(preShift & 0xFFFFFFFF);
  com->id = static_cast<uint32_t>(preShift >> 32);
  return true;
}

// I would use the function for testing code...
void Command::wait_for_empty() {
  Event e;
  while (command_queue.size() != 0) {
    e.wait_for(5000);
  }
}

void Command::flush() {
  uint64_t tmp;
  while (command_queue.dequeue(&tmp)) {}
}

std::mutex Command::error_flag_mutex;

// Used in set_error_flag to not flood the command queue
// See the .h for more explanation
void Command::set_error_flag(Network_Command_ID id, uint32_t value) {
  // Satisfy TSAN. There is a potential data race on `first_time`, even though its not a dangerous one.
  error_flag_mutex.lock();  
  // Initialize if this is the first function call
  static bool first_time = 1;
  if (first_time) {
    // Setup error flag timers to -1
    for (int i = 0; i < FLAGS_PER_ERROR * 6; i++) {
      Command::error_flag_timers[i] = -1000000;  // negative 1 second. 
    }
    first_time = 0;
  }
  error_flag_mutex.unlock();

  int error_index = (id-Command::SET_ADC_ERROR) * FLAGS_PER_ERROR;

  for (int i = 0, j = 1; i < FLAGS_PER_ERROR; i++, j*=2) {  // Go through each bit of the flag  
    if (value & ((uint32_t) j)) {  // if the specific bit is on
      // Determine the time delta, use the error_flag_timers
      int64_t delta = Utils::microseconds() - error_flag_timers[error_index + i]; 
      if (delta > 1000000) {  // Delta is greater than 1 second. This means we only send once per second!
        error_flag_timers[error_index + i] = Utils::microseconds();
        Command::put(id, (value & (uint32_t)j));  // put command on queue
      }
    }
  }
}

std::string Command::get_network_command_ID_string(uint command) {
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
    "RESET_PRU",
    "SENTINEL -- INVALID COMMAND",
  };
  return commands[command];
}

std::string Command::get_network_command_value_string(Network_Command * com) {
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
      "ADC_POSITIVE_SANITY_ERROR",
      "ADC_NEGATIVE_SANITY_ERROR",
      "ADC_SENTINEL",
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
      "CAN_BMS_LV_OVER_VOLTAGE_ERROR",
      "CAN_BMS_LV_UNDER_VOLTAGE_ERROR",
      "CAN_BMS_INTERNAL_OVER_TEMPERATURE",
      "CAN_BMS_DTC1_FAULT",
      "CAN_BMS_DTC2_FAULT",
      "CAN_MOTOR_CONTROLLER_FAULT",
      "CAN_MOTOR_CONTROLLER_WARN",
      "CAN_BMS_ROLLING_COUNTER_ERROR",
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
      "PRU_WATCHDOG_FAIL",
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

