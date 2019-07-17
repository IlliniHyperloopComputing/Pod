#include "PRUManager.h"

using Utils::print;
using Utils::LogLevel;
using Command::set_error_flag;

std::string PRUManager::name() {
  return "pru";
}

bool PRUManager::initialize_source() {
  /* Open the rpmsg_pru character device file */
  pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

  orange_diff_counter = 0;
  wheel_diff_counter = 0;
  reset_timeout_start = microseconds();

  if (pollfds[0].fd < 0) {
    print(LogLevel::LOG_ERROR, "PRU FAILED TO OPEN %s\n", DEVICE_NAME);
    set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_SETUP_FAILURE);
    return false;
  }

  int result = write(pollfds[0].fd, "start", 6);
  if (result == 0) {
    print(LogLevel::LOG_ERROR, "PRU Unable to write during init: %s\n", DEVICE_NAME);
    set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_SETUP_FAILURE);
    return false;
  }

  /* Poll until we receive a message from the PRU and then print it */
  result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
  if (result == 0) {
    print(LogLevel::LOG_ERROR, "PRU Unable to read during init: %s\n", DEVICE_NAME);
    set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_SETUP_FAILURE);
    return false;
  }

  print(LogLevel::LOG_INFO, "PRU Manager setup successful\n");
  return true;
}

void PRUManager::stop_source() {
  close(pollfds[0].fd);
  print(LogLevel::LOG_INFO, "PRU Manager stopped\n");
}

std::shared_ptr<PRUData> PRUManager::refresh() {

  reset_mutex.lock();
  if (do_reset) {
    int result = write(pollfds[0].fd, "RESETING\n", 9);
    if (result == 0) {
      print(LogLevel::LOG_ERROR, "PRU RESETTING ERROR Unable to write during operation %s\n", DEVICE_NAME);
      set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_WRITE_ERROR);
      // Error. return garbage
      return empty_data();
    }
   
    result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
    if (result == 0) {
      print(LogLevel::LOG_ERROR, "PRU RESETTING ERROR  Unable to read during operation. Read 0 bytes from PRU \n");
      set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_READ_ERROR);
      // Error. return garbage
      return empty_data();
    } else if (result != 9) {  // STARTING   the pru sends back "Starting\n", which is length 9
      print(LogLevel::LOG_ERROR, "PRU RESETTING error  Read wrong ammount from PRU %s\n", DEVICE_NAME);
      print(LogLevel::LOG_ERROR, "PRU RESETTING error Read: %d, should have read sizeof(RawPRUData)=%d\n", result, sizeof(RawPRUData));
      set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_READ_ERROR);
      // Error. return garbage
      return empty_data();
    } else {
      print(LogLevel::LOG_INFO, "PRU RESET SUCCESSFULL\n");
    }
    reset_timeout_start = microseconds();
    do_reset = false;
  }
  reset_mutex.unlock();

  // Write to PRU to trigger it to send us data
  // We send it a "1". This is arbitrary. 
  int result = write(pollfds[0].fd, "1", 2);
  if (result == 0) {
    print(LogLevel::LOG_ERROR, "Unable to write during operation %s\n", DEVICE_NAME);
    set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_WRITE_ERROR);
    // Error. return garbage
    return empty_data();
  }

  // Read from the PRU
  result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
  if (result == 0) {
    print(LogLevel::LOG_ERROR, "Unable to read during operation. Read 0 bytes from PRU \n");
    set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_READ_ERROR);
    // Error. return garbage
    return empty_data();
  } else if (result != sizeof(RawPRUData)) {
    print(LogLevel::LOG_ERROR, "Read wrong ammount from PRU %s\n", DEVICE_NAME);
    print(LogLevel::LOG_ERROR, "Read: %d, should have read sizeof(RawPRUData)=%d\n", result, sizeof(RawPRUData));
    set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_READ_ERROR);
    // Error. return garbage
    return empty_data();
  }

  // Copy Raw Data into Buffer
  RawPRUData raw_data;
  memcpy(&raw_data, readBuf, sizeof(RawPRUData));

  // Convert Raw Data into usable data
  PRUData new_data;
  
  // Convert orange encoder data
  for (int i = 0; i < NUM_ORANGE_INPUTS; i++) {
    new_data.orange_distance[i] = (int32_t) (raw_data.counts[orange_idx[i]] * orange_map[i]);
    new_data.orange_velocity[i] = convert_to_velocity(raw_data.decays[orange_idx[i]], 
                                                        raw_data.deltas[orange_idx[i]],
                                                        orange_map[i]);
  }

  // Convert wheel encoder data
  for (int i = 0; i < NUM_WHEEL_INPUTS; i++) {
    new_data.wheel_distance[i] = (int32_t) (raw_data.counts[wheel_idx[i]] * wheel_map[i]);
    new_data.wheel_velocity[i] = convert_to_velocity(raw_data.decays[wheel_idx[i]], 
                                                      raw_data.deltas[wheel_idx[i]],
                                                      wheel_map[i]);
  }

  // Get WATCHDOG
  const int watchdog_input_pin = 3;
  new_data.watchdog_hz = convert_to_velocity(raw_data.decays[watchdog_input_pin], 
                                              raw_data.deltas[watchdog_input_pin],
                                              1);  // 1 for distance, so we get hz


  // Store in shared_ptr
  std::shared_ptr<PRUData> ret_data = std::make_shared<PRUData>();
  *ret_data = new_data;

  return ret_data;
}

inline
int32_t PRUManager::convert_to_velocity(uint32_t decay, uint32_t delta, uint32_t distance) {
  // Pick the one that gives us the slower velocity
  uint32_t slower = std::max(decay, delta);

  if (slower == UINT32_MAX) {  // register this as 0 velocity
    return 0;
  } else {
    // Do the proper conversion into actual units. distance over time. 
    double time_diff = slower * CLOCK_TO_SEC;  // Convert from clocks to seconds
    return distance / time_diff;  // divide distance by time to get a velocity
  }
}

std::shared_ptr<PRUData> PRUManager::refresh_sim() {
  #ifdef SIM
  return SimulatorManager::sim.sim_get_pru();
  #else
  return empty_data();
  #endif
}

void PRUManager::initialize_sensor_error_configs() {
  if (!(ConfiguratorManager::config.getValue("error_orange_diff", error_orange_diff) && 
        ConfiguratorManager::config.getValue("error_orange_diff_count", error_orange_diff_count ) &&
        ConfiguratorManager::config.getValue("error_encoder_wheel_diff_count", error_encoder_wheel_diff_count) &&
        ConfiguratorManager::config.getValue("error_watchdog_heartbeat_min_hz", error_watchdog_heartbeat_min_hz) &&
      ConfiguratorManager::config.getValue("error_encoder_wheel_diff", error_encoder_wheel_diff))) { 
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: PRUManager Missing necessary configuration\n");
    exit(1);
  }
  // Define some sort of difference / variance that indicates that shit broke
}

void PRUManager::reset_pru() {
  reset_mutex.lock();
  do_reset = true;
  reset_mutex.unlock();
}

void PRUManager::check_for_sensor_error(const std::shared_ptr<PRUData> & check_data, E_States state) {
  // hardcoded for two of each type of sensor right now, could use standard dev?
  // just using distance for now, as velocity is based off of distance
  if (state == E_States::ST_FLIGHT_ACCEL ||
      state == E_States::ST_FLIGHT_BRAKE || 
      state == E_States::ST_FLIGHT_COAST) {

    // Orange check
    if (abs(check_data->orange_distance[0] - check_data->orange_distance[1]) > error_orange_diff) {
      orange_diff_counter++;
      if (orange_diff_counter > error_orange_diff_count) {
        set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR,PRUErrors::PRU_ORANGE_DIFF_ERROR);
      }
    } else {
      orange_diff_counter = 0;
    }
    
    // Wheel check
    if (abs(check_data->wheel_distance[0] - check_data->wheel_distance[1]) > error_encoder_wheel_diff) {
      wheel_diff_counter++;
      if (wheel_diff_counter > error_encoder_wheel_diff_count) {
        set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR,PRUErrors::PRU_WHEEL_DIFF_ERROR);
      }
    } else {
      wheel_diff_counter = 0;
    }
  }

  // Wait 2 second after reset, to avoid transient errors
  if (microseconds() - reset_timeout_start > 2000000 ) {
    if (check_data->watchdog_hz < error_watchdog_heartbeat_min_hz) {
      set_error_flag(Command::Network_Command_ID::SET_PRU_ERROR, PRUErrors::PRU_WATCHDOG_FAIL);
    }
  }

}



