#include "PRUManager.hpp"

using namespace Utils;

void PRUManager::initialize(){

  // MUST call SourceManagerBase initialize
  SourceManagerBase::initialize();
  
  /* Open the rpmsg_pru character device file */
  pollfds[0].fd = open(DEVICE_NAME, O_RDWR);

  if (pollfds[0].fd < 0) {
    print(LogLevel::LOG_ERROR, "FAILED TO OPEN %s\n", DEVICE_NAME);
    return;
  }

  int result = write(pollfds[0].fd, "start", 6);
  if(result == 0){
    print(LogLevel::LOG_ERROR, "Unable to write during init: %s\n", DEVICE_NAME);
    return;
  }

  /* Poll until we receive a message from the PRU and then print it */
  result = read(pollfds[0].fd, readBuf, MAX_BUFFER_SIZE);
  if(result == 0){
    print(LogLevel::LOG_ERROR, "Unable to read during init: %s\n", DEVICE_NAME);
    return;
  }

  initalized = true;

}

std::shared_ptr<PRUData> PRUManager::refresh() {

  // If not initialized, just return garbage
  if(!initalized){
    std::shared_ptr<PRUData> new_data = std::make_shared<PRUData>();
    memset(new_data.get(), (uint8_t)0, sizeof(PRUData));
    return new_data;
  }

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

  // Copy real data into buffer
  std::shared_ptr<PRUData> new_data = std::make_shared<PRUData>();
  memcpy(new_data.get(), readBuf, sizeof(PRUData));

  return new_data;
}
