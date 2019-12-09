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

#include <boost/python.hpp>
BOOST_PYTHON_MODULE(command){
        using namespace boost::python;

        // basic Command_Queue functions
        def("put", Command::put);
        def("get", Command::get);
        def("wait_for_empty", Command::wait_for_empty);
        def("set_error_flag", Command::set_error_flag);

        // exposes Command.h
        class_<Command::Network_Command>("Network_Command")
        .def_readwrite("id", &Command::Network_Command::id)
        .def_readwrite("value", &Command::Network_Command::value);

        // exposes Defines.hpp
        class_<ADCData>("ADCData")
        .def_readonly("data", &ADCData::data);
        // array should be exposed in another way because python doesn't have type array
        // here temporarily use readonly

        class_<CANData>("CANData")
        .def_readwrite("status_word", &CANData::status_word)
        .def_readwrite("position_val", &CANData::position_val)
        .def_readwrite("torque_val", &CANData::torque_val)
        .def_readwrite("controller_temp", &CANData::controller_temp)
        .def_readwrite("motor_temp", &CANData::motor_temp)
        .def_readwrite("dc_link_voltage", &CANData::dc_link_voltage)
        .def_readwrite("logic_power_supply_voltage", &CANData::logic_power_supply_voltage)
        .def_readwrite("current_demand", &CANData::current_demand)
        .def_readwrite("motor_current_val", &CANData::motor_current_val)
        .def_readwrite("electrical_angle", &CANData::electrical_angle)
        .def_readwrite("phase_a_current", &CANData::phase_a_current)
        .def_readwrite("phase_b_current", &CANData::phase_b_current)
        .def_readwrite("internal_relay_state", &CANData::internal_relay_state)
        .def_readwrite("relay_state", &CANData::relay_state)
        .def_readwrite("rolling_counter", &CANData::rolling_counter)
        .def_readwrite("fail_safe_state", &CANData::fail_safe_state)
        .def_readwrite("pack_current", &CANData::pack_current)
        .def_readwrite("pack_voltage_inst", &CANData::pack_voltage_inst)
        .def_readwrite("pack_voltage_open", &CANData::pack_voltage_open)
        .def_readwrite("pack_soc", &CANData::pack_soc)
        .def_readwrite("pack_amphours", &CANData::pack_amphours)
        .def_readwrite(" pack_resistance", &CANData:: pack_resistance)
        .def_readwrite("pack_dod", &CANData::pack_dod)
        .def_readwrite("pack_soh", &CANData::pack_soh)
        .def_readwrite("current_limit_status", &CANData::current_limit_status)
        .def_readwrite("max_pack_dcl", &CANData::max_pack_dcl)
        .def_readwrite("avg_pack_current", &CANData::avg_pack_current)
        .def_readwrite("highest_temp", &CANData::highest_temp)
        .def_readwrite("highest_temp_id", &CANData::highest_temp_id)
        .def_readwrite("avg_temp", &CANData::avg_temp)
        .def_readwrite("internal_temp", &CANData::internal_temp)
        .def_readwrite("low_cell_voltage", &CANData::low_cell_voltage)
        .def_readwrite("low_cell_voltage_id", &CANData::low_cell_voltage_id)
        .def_readwrite("high_cell_voltage", &CANData::high_cell_voltage)
        .def_readwrite("high_cell_voltage_id", &CANData::high_cell_voltage_id)
        .def_readwrite("low_cell_internalR", &CANData::low_cell_internalR)
        .def_readwrite("low_cell_internalR_id", &CANData::low_cell_internalR_id)
        .def_readwrite("high_cell_internalR", &CANData::high_cell_internalR)
        .def_readwrite("high_cell_internalR_id", &CANData::high_cell_internalR_id)
        .def_readwrite("power_voltage_input", &CANData::power_voltage_input)
        .def_readwrite("dtc_status_one", &CANData::dtc_status_one)
        .def_readwrite("dtc_status_two", &CANData::dtc_status_two)
        .def_readwrite("adaptive_total_cap", &CANData::adaptive_total_cap)
        .def_readwrite("adaptive_amphours", &CANData::adaptive_amphours)
        .def_readwrite("adaptive_soc", &CANData::adaptive_soc);

        class_<BMSCellBroadcastData>("BMSCellBroadcastData")
        .def_readwrite("cell_id", &BMSCellBroadcastData::cell_id)
        .def_readwrite("instant_voltage", &BMSCellBroadcastData::instant_voltage)
        .def_readwrite("internal_resistance", &BMSCellBroadcastData::internal_resistance)
        .def_readwrite("open_voltage", &BMSCellBroadcastData::open_voltage)
        .def_readwrite("checksum", &BMSCellBroadcastData::checksum);

        class_<BMSCells>("BMSCells")
        .def_readonly("cell_data", &BMSCells::cell_data)
        .def_readwrite("num_therms_enabled", &BMSCells::num_therms_enabled)
        .def_readwrite("lowest_therm_value", &BMSCells::lowest_therm_value)
        .def_readwrite("highest_therm_value", &BMSCells::highest_therm_value)
        .def_readwrite("highest_therm_id", &BMSCells::highest_therm_id)
        .def_readwrite("lowest_therm_id", &BMSCells::lowest_therm_id)
        .def_readwrite("PADDING", &BMSCells::PADDING)
        .def_readwrite("PADDING2", &BMSCells::PADDING2)
        .def_readwrite("PADDING3", &BMSCells::PADDING3)
        .def_readonly("therm_value", &BMSCells::therm_value);

        class_<I2CData>("I2CData")
        .def_readonly("temp", &I2CData::temp)
        .def_readwrite("pressure_sensor", &I2CData::pressure_sensor)
        .def_readwrite("temp_sensor", &I2CData::temp_sensor);

        class_<PRUData>("PRUData")
        .def_readonly("orange_distance", &PRUData::orange_distance)
        .def_readonly("orange_velocity", &PRUData::orange_velocity)
        .def_readonly("wheel_distance", &PRUData::wheel_distance)
        .def_readonly("wheel_velocity", &PRUData::wheel_velocity)
        .def_readwrite("watchdog_hz", &PRUData::watchdog_hz);

        class_<MotionData>("MotionData")
        .def_readonly("x", &MotionData::x)
        .def_readwrite("p_timeout", &MotionData::p_timeout)
        .def_readwrite("a_timeout", &MotionData::a_timeout)
        .def_readwrite("c_timeout", &MotionData::c_timeout)
        .def_readwrite("b_timeout", &MotionData::b_timeout)
        .def_readwrite("p_counter", &MotionData::p_counter)
        .def_readwrite("a_counter", &MotionData::a_counter)
        .def_readwrite("c_counter", &MotionData::c_counter)
        .def_readwrite("b_counter", &MotionData::b_counter)
        .def_readwrite("motor_state", &MotionData::motor_state)
        .def_readwrite("brake_state", &MotionData::brake_state)
        .def_readwrite("motor_target_torque", &MotionData::motor_target_torque)
        .def_readonly("relay_state_buf", &MotionData::relay_state_buf);
}