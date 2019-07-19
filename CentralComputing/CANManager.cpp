#include "CANManager.h"

bool CANManager::initialize_source() {
  relay_state_buf[0] = 0;
  relay_state_buf[1] = 0;
  relay_state_buf[2] = 0;
  memset(&stored_data, 0, sizeof(CANData));
  #ifndef BBB
  print(LogLevel::LOG_ERROR, "CAN Manager setup failed, not on BBB\n");
  return false;
  #endif

  // Setup structs that will be used below
  memset(&ifr,   0, sizeof(ifr));
  memset(&addr,  0, sizeof(addr));

  // Open CAN_RAW socket
  if ((can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_SETUP_FAILURE);
    print(LogLevel::LOG_ERROR, "CAN Socket creation failed. %s\n", strerror(errno));
    return false;
  }

  // Convert interface string into interface index
  snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "can0");
  if (ioctl(can_fd, SIOCGIFINDEX, &ifr) == -1) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_SETUP_FAILURE);
    print(LogLevel::LOG_ERROR, "CAN ioctl SIOCGIFINDEX failed. %s\n", strerror(errno));
    return false;
  }

  // Setup address for Bind
  addr.can_ifindex = ifr.ifr_ifindex;
  addr.can_family  = PF_CAN;

  // Bind
  if (bind(can_fd, (struct sockaddr *) & addr, sizeof(addr)) == -1) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_SETUP_FAILURE);
    print(LogLevel::LOG_ERROR, "CAN bind failed. %s\n", strerror(errno));
    return false;
  }

  // Setup recv_frame() variables
  iov.iov_base = &r_frame;
  msg.msg_name = &addr;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = &ctrlmsg;

  print(LogLevel::LOG_INFO, "CAN Manager setup successful\n");
  return true;
}

void CANManager::stop_source() {
  close(can_fd);
  print(LogLevel::LOG_INFO, "CAN Manager stopped\n");
}

bool CANManager::send_frame(uint32_t can_id, const char * buf, int len) {
  // Populate frame
  s_frame.can_id = can_id;
  memcpy(reinterpret_cast<char*>(s_frame.data), buf, len);
  // print(LogLevel::LOG_INFO, "CAN frame id: %x data:%x %x %x \n",
  //                           can_id, s_frame.data[0], s_frame.data[1], s_frame.data[2]);
  
  s_frame.can_dlc = len;
  // Write s_frame
  int ret = write(can_fd, &s_frame, sizeof(s_frame));
  // Check for errors
  if (ret == -1) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_SEND_FRAME_ERROR);
    print(LogLevel::LOG_ERROR, "CAN send_frame failed. %s\n", strerror(errno));
    return false;
  } else if (ret == 0) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_SEND_FRAME_ERROR);
    print(LogLevel::LOG_ERROR, "CAN send_frame sent 0 bytes\n");
    return false;
  }
  return true;
}

bool CANManager::recv_frame() {
  // https://stackoverflow.com/questions/32593697/understanding-the-msghdr-structure-from-sys-socket-h#32594071

  iov.iov_len = sizeof(r_frame);
  msg.msg_namelen = sizeof(addr);
  msg.msg_controllen = sizeof(ctrlmsg);
  msg.msg_flags = 0;

  int byte_count = recvmsg(can_fd, &msg, MSG_DONTWAIT); 

  if (byte_count == -1) {
    r_frame.len = 0;
    r_frame.can_id = 0;
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      // print(LogLevel::LOG_DEBUG, "CAN eagain ewouldblock. %s\n", strerror(errno));
      return true;
    } else {
      Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_RECV_FRAME_ERROR);
      print(LogLevel::LOG_ERROR, "CAN recvmsg failed. %s\n", strerror(errno));
      return false;
    }
  }

  // Check to make sure we read a full message
  // See linux/can.h for more details on these constants
  if ((size_t)byte_count == CAN_MTU) {
    // print(LogLevel::LOG_DEBUG, "Can MTU\n");
  } else if ((size_t)byte_count == CANFD_MTU) {
    // print(LogLevel::LOG_DEBUG, "CanFD MTU\n");
  } else {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_RECV_FRAME_ERROR);
    print(LogLevel::LOG_ERROR, "CAN recvmsg failed, incomplete CAN frame. \n");
    return false;
  }
  return true;
}

std::shared_ptr<CANData> CANManager::refresh() {
  std::shared_ptr<CANData> new_data = std::make_shared<CANData>();
  memcpy(new_data.get(), &stored_data, sizeof(CANData));   // This way, most of the data isn't zeros all of the time

  int64_t a = Utils::microseconds();
  // Send HV battery relay state frame
  // print(LogLevel::LOG_INFO, "CAN relay state %d %d %d \n", 
  //                          relay_state_buf[0], relay_state_buf[1], relay_state_buf[2]);

  send_mutex.lock();  // Used to protect socketfd (TSan datarace)
  if (!send_frame(can_id_bms_relay, (relay_state_buf), 3)) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_SEND_FRAME_ERROR);
  }
  send_mutex.unlock();  // Used to protect socketfd (TSan datarace)
  int64_t b = Utils::microseconds();
  // print(LogLevel::LOG_INFO, "CAN send_frame takes %lu microseconds\n", b-a);

  // Recieve frame(s). Populates variable r_frame
  do {
    a = Utils::microseconds();
    if (!recv_frame()) {
      // Error has already been done by recv_frame()
      // Nothing to do now but continue
      continue;
    }
    b = Utils::microseconds();
    // print(LogLevel::LOG_INFO, "CAN recv_frame takes %lu microseconds\n", b-a);
    if (r_frame.can_id == can_id_t1) {
      new_data->status_word =  cast_to_u32(0, 2, r_frame.data);
      new_data->position_val = cast_to_u32(2, 4, r_frame.data); 
      new_data->torque_val =   cast_to_u32(6, 2, r_frame.data);  
      // print(LogLevel::LOG_INFO, "CAN frame Motor Controller %d \n", can_id_t1);
      // print(LogLevel::LOG_INFO, "\tstatus word %d\n", new_data->status_word);
      // print(LogLevel::LOG_INFO, "\tposition value %d\n", new_data->position_val);
      // print(LogLevel::LOG_INFO, "\ttorque value %d\n", new_data->torque_val);
    } else if (r_frame.can_id == can_id_t2) {
      new_data->controller_temp =            r_frame.data[0];
      new_data->motor_temp =                 r_frame.data[1];
      new_data->dc_link_voltage =            cast_to_u32(2, 2, r_frame.data);
      new_data->logic_power_supply_voltage = cast_to_u32(4, 2, r_frame.data);
      new_data->current_demand =             cast_to_u32(6, 2, r_frame.data);
      // print(LogLevel::LOG_INFO, "CAN frame Motor Controller %d \n", can_id_t1);
      // print(LogLevel::LOG_INFO, "\tcontroller temp %d\n", new_data->controller_temp);
      // print(LogLevel::LOG_INFO, "\tmotor temperature %d\n", new_data->motor_temp);
      // print(LogLevel::LOG_INFO, "\tdc link voltage %d\n", new_data->dc_link_voltage);
      // print(LogLevel::LOG_INFO, "\tlogic power supply voltage %d\n", new_data->logic_power_supply_voltage);
      // print(LogLevel::LOG_INFO, "\tcurrent demand %d\n", new_data->current_demand);
    } else if (r_frame.can_id == can_id_t3) {
      new_data->motor_current_val = r_frame.data[0];
      new_data->electrical_angle =  cast_to_u32(2, 2, r_frame.data);
      new_data->phase_a_current =   cast_to_u32(4, 2, r_frame.data);
      new_data->phase_b_current =   cast_to_u32(6, 2, r_frame.data);
      // print(LogLevel::LOG_INFO, "CAN frame Motor Controller %d \n", can_id_t1);
      // print(LogLevel::LOG_INFO, "\tmotor current val %d\n", new_data->motor_current_val);
      // print(LogLevel::LOG_INFO, "\telectrical angle %d\n", new_data->electrical_angle);
      // print(LogLevel::LOG_INFO, "\tphase a current  %d\n", new_data->phase_a_current);
      // print(LogLevel::LOG_INFO, "\tphase b current %d\n", new_data->phase_b_current);
    } else if (r_frame.can_id == 0x6b0) {
      new_data->pack_current        = cast_to_u32(0, 2, r_frame.data);
      new_data->pack_voltage_inst   = cast_to_u32(2, 2, r_frame.data);
      new_data->pack_soc            = cast_to_u32(4, 1, r_frame.data);
      new_data->relay_state         = cast_to_u32(5, 2, r_frame.data);
      new_data->rolling_counter     = cast_to_u32(7, 1, r_frame.data);
    } else if (r_frame.can_id == 0x6b1) {
      new_data->fail_safe_state       = cast_to_u32(0, 2, r_frame.data);
      new_data->current_limit_status  = cast_to_u32(2, 2, r_frame.data);
      new_data->high_cell_voltage     = cast_to_u32(4, 2, r_frame.data);
      new_data->low_cell_voltage       = cast_to_u32(6, 2, r_frame.data);
    } else if (r_frame.can_id == 0x6b2) {
      new_data->dtc_status_one        = cast_to_u32(0, 2, r_frame.data);
      new_data->dtc_status_two        = cast_to_u32(2, 2, r_frame.data);
      new_data->power_voltage_input   = cast_to_u32(4, 2, r_frame.data);
      new_data->highest_temp          = cast_to_u32(6, 1, r_frame.data);
      new_data->internal_temp         = cast_to_u32(7, 1, r_frame.data);
    } else if (r_frame.can_id == 0x6b3) {
      new_data->pack_voltage_open     = cast_to_u32(0, 2, r_frame.data);
      new_data->pack_amphours         = cast_to_u32(2, 2, r_frame.data);
      new_data->pack_resistance       = cast_to_u32(4, 2, r_frame.data);
      new_data->pack_dod              = cast_to_u32(6, 1, r_frame.data);
      new_data->pack_soh              = cast_to_u32(7, 1, r_frame.data);
    } else if (r_frame.can_id == 0x6b4) {
      new_data->max_pack_dcl          = cast_to_u32(0, 2, r_frame.data);
      new_data->avg_pack_current      = cast_to_u32(2, 2, r_frame.data);
      new_data->avg_temp              = cast_to_u32(4, 1, r_frame.data);
      new_data->high_cell_voltage_id  = cast_to_u32(5, 1, r_frame.data);
      new_data->low_cell_voltage_id   = cast_to_u32(6, 1, r_frame.data);
      new_data->highest_temp_id       = cast_to_u32(7, 1, r_frame.data);
    } else if (r_frame.can_id == 0x6b5) {
      new_data->low_cell_internalR      = cast_to_u32(0, 2, r_frame.data);
      new_data->high_cell_internalR     = cast_to_u32(2, 2, r_frame.data);
      new_data->low_cell_internalR_id   = cast_to_u32(4, 1, r_frame.data);
      new_data->high_cell_internalR_id  = cast_to_u32(5, 1, r_frame.data);
    } else if (r_frame.can_id == 0x6b6) {
      new_data->adaptive_total_cap      = cast_to_u32(0, 2, r_frame.data);
      new_data->adaptive_amphours       = cast_to_u32(2, 2, r_frame.data);
      new_data->adaptive_soc            = cast_to_u32(4, 1, r_frame.data);
    } else if (r_frame.can_id == 0x6b7) {  // Cell data
      // Verify that it has a valid ID that we can use to index
      if (r_frame.data[0] < 30) {
        int cell_id = r_frame.data[0]; // If its "1" indexed instead of 0, incremnt this
        private_cell_data.cell_data[cell_id].cell_id = r_frame.data[0];
        private_cell_data.cell_data[cell_id].instant_voltage = cast_to_u32(1, 2, r_frame.data);
        private_cell_data.cell_data[cell_id].internal_resistance = cast_to_u32(3, 2, r_frame.data);
        private_cell_data.cell_data[cell_id].open_voltage = cast_to_u32(5, 2, r_frame.data);
        private_cell_data.cell_data[cell_id].checksum = r_frame.data[7];
      } else {
        print(LogLevel::LOG_ERROR, "Cell Data CAN frame has bad ID ??? %d", r_frame.data[0] ); 
      }
     
    // Thermistor https://www.orionbms.com/downloads/misc/thermistor_module_canbus.pdf
    } else if (r_frame.can_id == 0x1838F380) {  // Thermistor General CAN
      // Verify that it has a valid ID that we can use to index
      int16_t therm_id = cast_to_u32(0, 2, r_frame.data);
      if (therm_id < 40) {
        private_cell_data.therm_value[therm_id] = r_frame.data[2];
        private_cell_data.num_therms_enabled = r_frame.data[3];
        private_cell_data.lowest_therm_value = r_frame.data[4];
        private_cell_data.highest_therm_value = r_frame.data[5];
        private_cell_data.highest_therm_id = r_frame.data[6];
        private_cell_data.lowest_therm_id = r_frame.data[7];
      } else {
        print(LogLevel::LOG_ERROR, "Therm Data CAN frame has bad ID ??? %d", therm_id); 
      }
    } 
    else {
      print(LogLevel::LOG_DEBUG, "CAN Frame UNKNOWN msg: id: %d, len: %d, \n", r_frame.can_id, r_frame.len); 
    }

    // Print the contents of r_frame (assumes len <= 8)
    // char buff[16];
    // for (int j = 0; j < r_frame.len*2; j+=2) {
    //   put_hex_byte(buff+j, r_frame.data[j/2]);
    // }
    // buff[r_frame.len*2] = '\0';  // include null terminator
    // 
    // print(LogLevel::LOG_INFO, "CAN msg: id: %d, len: %d, data: %s\n", r_frame.can_id, r_frame.len, buff); 
  } while (r_frame.len != 0);

  memcpy(&stored_data, new_data.get(), sizeof(CANData));   // Copy new data to stored data

  // Copy the "private" data to the public facing variable
  cell_data_mutex.lock();
  memcpy(&public_cell_data, &private_cell_data, sizeof(BMSCells));
  cell_data_mutex.unlock();

  return new_data;
}

std::shared_ptr<CANData> CANManager::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_can();
  #else
  return empty_data();
  #endif
}

void CANManager::initialize_sensor_error_configs() {
  if (!(ConfiguratorManager::config.getValue("error_motor_ctrl_over_temp", error_motor_ctrl_over_temp) && 
      ConfiguratorManager::config.getValue("error_motor_over_temp", error_motor_over_temp) &&
      ConfiguratorManager::config.getValue("error_dc_link_over_voltage", error_dc_link_over_voltage) &&
      ConfiguratorManager::config.getValue("error_dc_link_under_voltage", error_dc_link_under_voltage) &&
      ConfiguratorManager::config.getValue("error_motor_ctrl_logic_over_voltage",  error_motor_ctrl_logic_over_voltage) &&  // NOLINT
      ConfiguratorManager::config.getValue("error_motor_ctrl_logic_under_voltage", error_motor_ctrl_logic_under_voltage) && // NOLINT
      ConfiguratorManager::config.getValue("error_cell_over_voltage",  error_cell_over_voltage) &&
      ConfiguratorManager::config.getValue("error_cell_under_voltage", error_cell_under_voltage) &&
      ConfiguratorManager::config.getValue("error_cell_over_temp", error_cell_over_temp) &&
      ConfiguratorManager::config.getValue("error_battery_over_voltage",  error_battery_over_voltage) &&
      ConfiguratorManager::config.getValue("error_battery_under_voltage", error_battery_under_voltage) &&
      ConfiguratorManager::config.getValue("error_battery_over_current", error_battery_over_current) &&
      ConfiguratorManager::config.getValue("error_bms_rolling_counter_timeout", error_bms_rolling_counter_timeout) &&
      ConfiguratorManager::config.getValue("error_bms_internal_over_temp",  error_bms_internal_over_temp) &&  // NOLINT
      ConfiguratorManager::config.getValue("error_bms_logic_over_voltage",  error_bms_logic_over_voltage) &&  // NOLINT
      ConfiguratorManager::config.getValue("error_bms_logic_under_voltage", error_bms_logic_under_voltage))) { // NOLINT
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: CANManager Missing necessary configuration\n");
    exit(1);
  }

  // rolling counter is only up to 255, so this is an unrealistic number for it
  // This guarantees that the first comparison is good
  rolling_counter_tracker = 1000; 
  // set the timer to compare right away;
  rolling_counter_timer = -1000000;
}

void CANManager::set_relay_state(HV_Relay_Select relay, HV_Relay_State state) {
  std::lock_guard<std::mutex> guard(send_mutex);  // Used to protect socketfd (TSan datarace)
  print(LogLevel::LOG_DEBUG, "RELAY STATE: %d! \n", state);
  relay_state_buf[relay] = state;
  // (reinterpret_cast<char*>(&relay_state_buf))[relay] = state;
}

void CANManager::set_motor_state(bool enable) {  // TODO: Need to Send controlword 3 at startup
  std::lock_guard<std::mutex> guard(send_mutex);  // Used to protect socketfd (TSan datarace)
  char bufferArray[8];
  if (enable) {
    bufferArray[0] = static_cast<char>(0x80);
    bufferArray[1] = 0x00;
    send_frame(0, bufferArray, 2);  // Send pre-operational
    Utils::busyWait(10000);

    bufferArray[0] = static_cast<char>(0x01);
    bufferArray[1] = 0x00;
    send_frame(0, bufferArray, 2);  // Send operational
    Utils::busyWait(50000);

    bufferArray[0] = static_cast<char>(0x06);
    bufferArray[1] = 0x00;
    bufferArray[2] = 0x00;
    bufferArray[3] = 0x00;
    bufferArray[4] = 0x00;
    bufferArray[5] = 0x00;
    bufferArray[6] = 0x00;
    bufferArray[7] = 0x00;
    send_frame(0x201, bufferArray, 8);  // Move motor  to read-to-switch on
    Utils::busyWait(50000);

    bufferArray[0] = static_cast<char>(0x07);
    bufferArray[1] = 0x00;
    bufferArray[2] = 0x00;
    bufferArray[3] = 0x00;
    bufferArray[4] = 0x00;
    bufferArray[5] = 0x00;
    bufferArray[6] = 0x00;
    bufferArray[7] = 0x00;
    send_frame(0x201, bufferArray, 8);  // Move motor  to switched-on
    Utils::busyWait(50000);

    bufferArray[0] = static_cast<char>(0x0F);
    bufferArray[1] = 0x00;
    bufferArray[2] = 0x00;
    bufferArray[3] = 0x00;
    bufferArray[4] = 0x00;
    bufferArray[5] = 0x00;
    bufferArray[6] = 0x00;
    bufferArray[7] = 0x00;
    send_frame(0x201, bufferArray, 8);  // Move motor operation enabled with/ PWM on
  } else {
    bufferArray[0] = static_cast<char>(0x06);
    bufferArray[1] = 0x00;
    bufferArray[2] = 0x00;
    bufferArray[3] = 0x00;
    bufferArray[4] = 0x00;
    bufferArray[5] = 0x00;
    bufferArray[6] = 0x00;
    bufferArray[7] = 0x00;
    send_frame(0x201, bufferArray, 8);  // Exit motor enabled
    Utils::busyWait(50000);

    bufferArray[0] = static_cast<char>(0x80);
    bufferArray[1] = 0x00;
    send_frame(0, bufferArray, 2);  // Send pre-operational
  }
}

void CANManager::set_motor_throttle(int16_t value) {  // Using Throttle Value Here
  std::lock_guard<std::mutex> guard(send_mutex);  // Used to protect socketfd (TSan datarace)
  char bufferArray[8];
  bufferArray[0] = static_cast<char>(0x0F);
  bufferArray[1] = 0x00;
  bufferArray[2] = 0x00;
  bufferArray[3] = 0x00;
  bufferArray[4] = 0x00;
  bufferArray[5] = 0x00;
  bufferArray[6] = (reinterpret_cast<char *>(&value))[0];
  bufferArray[7] = (reinterpret_cast<char *>(&value))[1];
  send_frame(0x201, bufferArray, 8);  // Move motor operation enabled with/ PWM on
}

void CANManager::check_for_sensor_error(const std::shared_ptr<CANData> & check_data, E_States state) {
  if (check_data->controller_temp > error_motor_ctrl_over_temp) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_INTERNAL_OVER_TEMPERATURE);
  }
  if (check_data->motor_temp > error_motor_over_temp) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_MOTOR_OVER_TEMPERATURE);
  }  
  if (check_data->dc_link_voltage > error_dc_link_over_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_HV_OVER_VOLTAGE_ERROR);
  } 
  if (check_data->dc_link_voltage < error_dc_link_under_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_HV_UNDER_VOLTAGE_ERROR);
  }
  if (check_data->logic_power_supply_voltage > error_motor_ctrl_logic_over_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_LV_OVER_VOLTAGE_ERROR);
  }
  if (check_data->logic_power_supply_voltage < error_motor_ctrl_logic_under_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_LV_UNDER_VOLTAGE_ERROR);
  }
  if (check_data->high_cell_voltage > error_cell_over_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_CELL_OVER_VOLTAGE);
  }
  if (check_data->low_cell_voltage < error_cell_under_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_CELL_UNDER_VOLTAGE);
  }
  if (check_data->highest_temp > error_cell_over_temp) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_CELL_OVER_TEMP);
  }
  if (check_data->pack_voltage_inst > error_battery_over_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_BATTERY_OVER_VOLTAGE);
  }
  if (check_data->pack_voltage_inst < error_battery_under_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_BATTERY_UNDER_VOLTAGE);
  }
  if (check_data->pack_voltage_open > error_battery_over_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_BATTERY_OVER_VOLTAGE);
  }
  if (check_data->pack_voltage_open < error_battery_under_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_BATTERY_UNDER_VOLTAGE);
  }
  if (check_data->pack_current > error_battery_over_current) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_BATTERY_OVER_CURRENT);
  }
  if (check_data->power_voltage_input > error_bms_logic_over_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_LV_OVER_VOLTAGE_ERROR);
  }
  if (check_data->power_voltage_input < error_bms_logic_under_voltage) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_LV_UNDER_VOLTAGE_ERROR);
  }
  if (check_data->internal_temp > error_bms_internal_over_temp) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_INTERNAL_OVER_TEMPERATURE);
  }
  // Refer here: https://www.orionbms.com/manuals/utility_o2/
  if (check_data->dtc_status_one != 0) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_DTC1_FAULT);
  }
  if (check_data->dtc_status_two != 0) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_DTC2_FAULT);
  }

  // Refer to eDrive_firmware_specifications , page 93
  if (check_data->status_word & 0x8 ) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_FAULT);
  }
  // If this error triggers too much, consider only having it trigger 
  // Within a flight mode
  if (check_data->status_word & 0x80) {
    Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_MOTOR_CONTROLLER_WARN);
  }

  // Rolling counter must increment every 100 milliseconds. We define a bms rolling counter timeout value
  // So we see if 100 milliseconds are up
  //    If yes, then we check if the rolling_counter_tracker == the rolling_counter 
  //      The Rolling counter _should_ be updated!  IF not, it means the BMS could have stalled
  //      The rolling_counter_tracker is updated every time the timer is up
  //    If no, then keep waiting
  if (microseconds()-rolling_counter_timer > error_bms_rolling_counter_timeout) {
    if(check_data->rolling_counter == rolling_counter_tracker) {
      Command::set_error_flag(Command::Network_Command_ID::SET_CAN_ERROR, CANErrors::CAN_BMS_ROLLING_COUNTER_ERROR);
    } 
    rolling_counter_tracker = check_data->rolling_counter;
    rolling_counter_timer = microseconds();
  }

}

// This will convert Big Endian data types to Little Endian types
inline uint32_t CANManager::cast_to_u32(int offset, int bytes_per_item, uint8_t * bufferArray) {
  uint32_t tmp = 0;
  for (int i = 0; i < bytes_per_item; i++) {
    tmp |= (uint8_t)(bufferArray[offset + i] << (i * 8));
  }
  return tmp;
}

inline uint64_t CANManager::cast_to_u64(int offset, int bytes_per_item, uint8_t * bufferArray) {
  uint64_t tmp = 0;
  for (int i = 0; i < bytes_per_item; i++) {
    tmp |= (uint8_t)(bufferArray[offset + i] << (i * 8));
  }
  return tmp;
}

void CANManager::u32_to_bytes(uint32_t toCast, char* bufferArray) {
  bufferArray[0] = toCast;
  bufferArray[1] = toCast >>  8;
  bufferArray[2] = toCast >> 16;
  bufferArray[3] = toCast >> 24;
}

void CANManager::u16_to_bytes(uint16_t toCast, char* bufferArray) {
  bufferArray[0] = toCast;
  bufferArray[1] = toCast >> 8;
}
void CANManager::i16_to_bytes(int16_t toCast, char* bufferArray) {
  bufferArray[0] = toCast;
  bufferArray[1] = toCast >> 8;
}
