#include "PRUManager.hpp"

using namespace Utils;

std::string PRUManager::name() {
  return "pru";
}

bool PRUManager::initialize_source(){
  
  /* Open the rpmsg_pru character device file */
  pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

  if (pollfds[0].fd < 0) {
    print(LogLevel::LOG_ERROR, "PRU FAILED TO OPEN %s\n", DEVICE_NAME);
    return false;
  }

  int result = write(pollfds[0].fd, "start", 6);
  if(result == 0){
    print(LogLevel::LOG_ERROR, "PRU Unable to write during init: %s\n", DEVICE_NAME);
    return false;
  }

  /* Poll until we receive a message from the PRU and then print it */
  result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
  if(result == 0){
    print(LogLevel::LOG_ERROR, "PRU Unable to read during init: %s\n", DEVICE_NAME);
    return false;
  }

  print(LogLevel::LOG_DEBUG, "PRU Manager setup successful\n");
  return true;
}

void PRUManager::stop_source(){
	close(pollfds[0].fd);
  print(LogLevel::LOG_DEBUG, "PRU Manager stopped\n");
}

std::shared_ptr<PRUData> PRUManager::refresh() {

  int result = write(pollfds[0].fd, "1", 2);
  if(result == 0){
    print(LogLevel::LOG_ERROR, "Unable to write during operation %s\n", DEVICE_NAME);

    // Error. return garbage
    std::shared_ptr<PRUData> new_data = std::make_shared<PRUData>();
    memset(new_data.get(), (uint8_t)-1, sizeof(PRUData));
    return new_data;
  }

	result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
  if(result == 0){
    print(LogLevel::LOG_ERROR, "Unable to read during operation %s\n", DEVICE_NAME);

    // Error. return garbage
    std::shared_ptr<PRUData> new_data = std::make_shared<PRUData>();
    memset(new_data.get(), (uint8_t)-1, sizeof(PRUData));
    return new_data;
  }

  // Copy Raw Data into Buffer
  RawPRUData raw_data;
  memcpy(&raw_data, readBuf, sizeof(RawPRUData));

  // Convert Raw Data into usable data
  PRUData new_data;
  
  // Convert encoder data
  for(int i = 0; i < NUM_ENC_INPUTS; i++){
    new_data.encoder_distance[i] = raw_data.counts[enc_idx[i]] * enc_map[i];
    new_data.encoder_velocity[i] = convert_to_velocity( raw_data.decays[enc_idx[i]], 
                                                        raw_data.deltas[enc_idx[i]],
                                                        enc_map[i]);
  }

  // Convert disk RPM data
  for(int i = 0; i < NUM_MOTOR_INPUTS; i++){
    new_data.disk_RPM[i] = convert_to_velocity( raw_data.decays[enc_idx[i]], 
                                                raw_data.deltas[enc_idx[i]],
                                                enc_map[i]);
  }

  // Store in shared_ptr
  std::shared_ptr<PRUData> ret_data = std::make_shared<PRUData>();
  *ret_data = new_data;

  return ret_data;
}

inline
double PRUManager::convert_to_velocity(uint32_t decay, uint32_t delta, double distance){

    // Pick the one that gives us the slower velocity
    uint32_t slower = std::max(decay, delta);

    if(slower == UINT32_MAX){ // register this as 0 velocity
      return 0;
    }
    else{
      // Do the proper conversion into m/s
      double time_diff = slower * CLOCK_TO_SEC;
      return distance / time_diff;
    }

}

std::shared_ptr<PRUData> PRUManager::refresh_sim() {
  return empty_data();
}
