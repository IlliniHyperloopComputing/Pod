#include "CANManager.hpp"

bool CANManager::initialize_source(){

  #ifndef BBB
  print(LogLevel::LOG_ERROR, "CAN Manager setup failed, not on BBB\n");
  return false;
  #endif

  memset(&ifr,   0, sizeof(ifr));
  memset(&addr,  0, sizeof(addr));

  // Open CAN_RAW socket
  if((can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) == -1){
    print(LogLevel::LOG_ERROR, "CAN Socket creation failed. %s\n", strerror(errno));
    return false;
  }

  //int flags = fcntl(can_fd, F_GETFL, 0);
  //if(flags == -1){
  //  print(LogLevel::LOG_ERROR, "CAN F_GETFL failed. %s\n", strerror(errno));
  //  return false;
  //}
  //flags |= O_NONBLOCK;
  //if(fcntl(can_fd, F_SETFL, flags) == -1){
  //  print(LogLevel::LOG_ERROR, "CAN F_SETFL failed. %s\n", strerror(errno));
  //  return false;
  //}

  // Convert interface string into interface index
  strcpy(ifr.ifr_name, "can0");
  if(ioctl(can_fd, SIOCGIFINDEX, &ifr) == -1){
    print(LogLevel::LOG_ERROR, "CAN ioctl SIOCGIFINDEX failed. %s\n", strerror(errno));
    return false;
  }

  // Setup address for Bind
  addr.can_ifindex = ifr.ifr_ifindex;
  addr.can_family  = PF_CAN;

  // Bind
  if(bind(can_fd, (struct sockaddr *) & addr, sizeof(addr)) == -1){
    print(LogLevel::LOG_ERROR, "CAN bind failed. %s\n", strerror(errno));
    return false;
  }

  print(LogLevel::LOG_DEBUG, "CAN Manger setup successful\n");
  return true;
}

void CANManager::stop_source(){
  close(can_fd);
  print(LogLevel::LOG_DEBUG, "CAN Manger stopped\n");
}

bool CANManager::send_frame(uint32_t can_id, const uint8_t * buf, int len){
  // Populate frame
  struct can_frame frame;
  memset(&frame, 0, sizeof(frame));
  frame.can_id = can_id;
  strncpy((char *)frame.data, (const char *)buf, (size_t)len);
  frame.can_dlc = len;
  // Write frame
  int ret = write(can_fd, &frame, sizeof(frame));
  // Check for errors
  if(ret == -1){
    print(LogLevel::LOG_ERROR, "CAN send_frame failed. %s\n", strerror(errno));
    return false;
  }
  else if(ret == 0){
    print(LogLevel::LOG_ERROR, "CAN send_frame sent 0 bytes\n");
    return false;
  }
  return true;
}

bool CANManager::recv_frame(){
  struct iovec iov;
  struct msghdr msg;
  struct canfd_frame frame;
  iov.iov_base = &frame;
  msg.msg_name = &addr;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_control = &ctrlmsg;
  //https://stackoverflow.com/questions/32593697/understanding-the-msghdr-structure-from-sys-socket-h#32594071

  //hotpath
  iov.iov_len = sizeof(frame);
  msg.msg_namelen = sizeof(addr);
  msg.msg_controllen = sizeof(ctrlmsg);
  msg.msg_flags = 0;

  int byte_count = recvmsg(can_fd, &msg, MSG_DONTWAIT); 

  if(byte_count == -1){
    if(errno == EAGAIN || errno == EWOULDBLOCK){
      print(LogLevel::LOG_DEBUG, "CAN eagain ewouldblock. %s\n", strerror(errno));
      return false;
    }
    else{
      print(LogLevel::LOG_ERROR, "CAN recvmsg failed. %s\n", strerror(errno));
      return false;
    }
  }

  int maxdlen = 0;

	if ((size_t)byte_count == CAN_MTU){
    maxdlen = CAN_MAX_DLEN;
  }
  else if ((size_t)byte_count == CANFD_MTU){
    maxdlen = CANFD_MAX_DLEN;
  }
  else {
    print(LogLevel::LOG_ERROR, "CAN recvmsg failed, incomplete CAN frame. \n");
    return false;
  }

  char buff[64];
  for(int j = 0; j < frame.len*2; j+=2){
    put_hex_byte(buff+j, frame.data[j/2]);
  }
  buff[frame.len*2] = '\0';

  print(LogLevel::LOG_INFO, "CAN msg: id: %d, len: %d, data: %s\n", frame.can_id, frame.len, buff);

  return true;

}

std::shared_ptr<CANData> CANManager::refresh() {
  std::shared_ptr<CANData> new_data = std::make_shared<CANData>();
  new_data->dummy_data = i;
  i++;

  recv_frame();
  send_frame(123, (const uint8_t *)"wow", 3);

  return new_data;
}

std::shared_ptr<CANData> CANManager::refresh_sim() {
  return empty_data();
}
