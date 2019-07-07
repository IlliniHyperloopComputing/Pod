#ifdef SIM  // Only compile if building test executable
#include "Simulator.h"

using Utils::print;
using Utils::LogLevel;

Simulator SimulatorManager::sim;

Simulator::Simulator() {
  connected_tcp.reset();
  connected_udp.reset();
  scenario = nullptr;
}

//
//
//
// TCP
//
//
//

int Simulator::start_server_tcp(const char * hostname, const char * port) {
  connected_tcp.reset();
  socketfd_tcp = socket(AF_INET, SOCK_STREAM, 0);
  // int blocking = 0; // 0 is blocking, 1 is non-blocking
  // ioctl(socketfd_tcp, FIONBIO, &blocking); 
  int enable = 1;
  if (setsockopt(socketfd_tcp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    perror("Sim - TCP - setsockopt(SO_REUSEADDR) failed");
  }

  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int s = getaddrinfo(hostname, port, &hints, &result);
  if (s != 0) {
    print(LogLevel::LOG_ERROR, "Sim - TCP - getaddrinfo: %s\n", gai_strerror(s));
    return -1;
  }
  if (bind(socketfd_tcp, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind");
    return -1;
  }
  if (listen(socketfd_tcp, 1) != 0) {
    perror("listen");
    return -1;
  }
  print(LogLevel::LOG_DEBUG, "Sim - TCP - Server setup successfully, socketfd_tcp : %d\n", socketfd_tcp);
  free(result);

  return socketfd_tcp;
}

int Simulator::accept_client_tcp() {
  struct pollfd p;
  p.fd = socketfd_tcp;
  p.events = POLLIN;
  int ret = 0;
  print(LogLevel::LOG_DEBUG, "Sim - Awaiting TCP connection from Pod\n");
  while (1) {
    ret = poll(&p, 1, 200);
    if (ret == 1) {  // there's something trying to connect, or we are exiting
      int tmp_clientfd = accept(socketfd_tcp, NULL, NULL);
      clientfd_tcp = tmp_clientfd;
      if (clientfd_tcp != -1) {
        print(LogLevel::LOG_DEBUG, "Sim - TCP Connected! \n"); 
      }
      return clientfd_tcp;
    }
  }
  return -1;
}

void Simulator::sim_connect_tcp() {
  closed_tcp.reset();
  pause_tcp.invoke();

  // try to connect to a client
  print(LogLevel::LOG_DEBUG, "Sim - Starting TCP network read thread\n");
  do_accept_client_tcp.store(true);
  while (do_accept_client_tcp.load() && accept_client_tcp() > 0) {
    active_connection.store(true);
    read_thread = std::thread([&]() { read_loop_tcp(); });
    connected_tcp.invoke();
    read_thread.join();
    pause_tcp.wait();  // Only waits if `pause_udp.reset()` is called
  } 
  print(LogLevel::LOG_DEBUG, "Sim - Pod TCP client exited. \n");
}

bool Simulator::send_command(std::shared_ptr<Command::Network_Command> command) {
  int bytes_written = write(clientfd_tcp, command.get(), sizeof(Command::Network_Command));
  // print(LogLevel::LOG_EDEBUG, "Sim - Bytes written : %d, ID : %d, Value : %d  
  //        clientfd_tcp : %d\n", bytes_written, command->id, command->value, clientfd_tcp);
  int size = sizeof(Command::Network_Command);
  return bytes_written == size;
}

void Simulator::read_loop_tcp() {
  while (active_connection.load()) {
    // dump the data because we don't need it or do anything with it.
    // TODO if we want to we can keep the data and use it for error checking purposes
    // but that seems redundant and like a lot of work

    char buf[100];
    read(clientfd_tcp, buf, 99);
  }
  closed_tcp.invoke();
}

void Simulator::disconnect_tcp() {
  active_connection.store(false);  // stop the read loop
  do_accept_client_tcp.store(false);  // stop the connection loop

  pause_tcp.invoke();  // Ensure this event doesn't wait
  shutdown(clientfd_tcp, SHUT_RDWR);
  closed_tcp.wait();    // wait for sim_connect_tcp() to close, which was waiting on the read_loop_tcp
  close(clientfd_tcp);  // close TCP connection
  shutdown(socketfd_tcp, SHUT_RDWR);
  close(socketfd_tcp);  // close TCP server
}

//
//
//
// UDP
//
//
//

bool Simulator::start_udp(const char * hostname, const char * send_port, const char * recv_port) {
  connected_udp.reset();

  int rv;
  int enable = 1;

  /////////////////////
  // SETUP UDP SEND PORT
  memset(&hints_udp, 0, sizeof(hints_udp));
  hints_udp.ai_family = AF_INET;
  hints_udp.ai_socktype = SOCK_DGRAM;
  
  // Get destination address
  if ((rv = getaddrinfo(hostname, send_port, &hints_udp, &sendinfo_udp)) != 0) {
    print(LogLevel::LOG_ERROR, "Sim - UDP send Error get addrinfo: %s\n", gai_strerror(rv));
    goto RETURN_ERROR;
  }

  // Create socket
  if ((socketfd_udp = socket(sendinfo_udp->ai_family, sendinfo_udp->ai_socktype, sendinfo_udp->ai_protocol)) == -1) {
    print(LogLevel::LOG_ERROR, "Sim - UDP send Error getting socket\n");
    goto RETURN_ERROR;
  }

  /////////////////////
  // SETUP UDP RECV PORT
  memset(&hints_udp, 0, sizeof(hints_udp));
  hints_udp.ai_family = AF_INET;
  hints_udp.ai_socktype = SOCK_DGRAM;
  hints_udp.ai_flags = AI_PASSIVE;
  
  // For recv address
  if ((rv = getaddrinfo(hostname, recv_port, &hints_udp, &recvinfo_udp)) != 0) {
    print(LogLevel::LOG_ERROR, "Sim - UDP Error getting addrinfo: %s\n", gai_strerror(rv));
    goto RETURN_ERROR;
  }

  // Allow port reuse
  if (setsockopt(socketfd_udp, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    print(LogLevel::LOG_ERROR, "Sim - UDP setsockopt(SO_REUSEADDR) failed: %s\n", strerror(errno));
    goto RETURN_ERROR;
  }

  // Bind to port (tells the OS the default incoming address)
  if (bind(socketfd_udp, recvinfo_udp->ai_addr, recvinfo_udp->ai_addrlen) != 0) {
    print(LogLevel::LOG_ERROR, "Sim - UDP bind failed: %s\n", strerror(errno));
    goto RETURN_ERROR;
  }

  // Connect (tells the OS the default destination)
  // Connect to the send address
  // if(connect(socketfd_udp, sendinfo_udp->ai_addr, sendinfo_udp->ai_addrlen) == -1) {
  //   print(LogLevel::LOG_ERROR, "UDP connecting failed: %s\n", strerror(errno));
  //   goto RETURN_ERROR;
  // }

  // freeaddrinfo(sendinfo_udp);
  freeaddrinfo(recvinfo_udp);
  return true;

RETURN_ERROR:
  print(LogLevel::LOG_ERROR, "Sim - UDP returning error on startup");
  close(socketfd_udp);
  freeaddrinfo(sendinfo_udp);
  freeaddrinfo(recvinfo_udp);
  return false;
}

int Simulator::udp_recv(uint8_t* recv_buf, uint8_t len) {
  socklen_t fromlen;                 // needed for recvfrom
  struct sockaddr_storage fromaddr;  // needed for recvfrom
  fromlen = sizeof fromaddr;         // needed for recvfrom

  int byte_count = recvfrom(socketfd_udp, recv_buf, len, 0, 
      (struct sockaddr *)&fromaddr, &fromlen);
  if (byte_count == -1) {
    print(LogLevel::LOG_DEBUG, "Sim - UDP recv failed: %s\n", strerror(errno));
    // TODO Determine if this is an error worth reporting
    return 0;
  }
  recv_buf[byte_count] = '\0';
  
  return byte_count;
}

bool Simulator::udp_parse(uint8_t* buf, uint8_t len) {
  if (buf[0] == 'A') {    // Check if we got an 'ACK'
    return true;          // 
  } else {
    // Print whatever we got because it wasn't PING
    print(LogLevel::LOG_DEBUG, "Sim - Parsed %d bytes (not an 'ACK'), they are: %s\n", len, buf);   }
  return false;
}

int Simulator::udp_send(uint8_t* buf, uint8_t len) { 
  int byte_count = sendto(socketfd_udp, buf, len, 0,
      sendinfo_udp->ai_addr, sendinfo_udp->ai_addrlen);
  if (byte_count == -1) {
    print(LogLevel::LOG_DEBUG, "Sim - UDP recv failed: %s\n", strerror(errno));
    // TODO Determine if this is an error worth reporting
    return 0;
  }
  return byte_count;
}

void Simulator::sim_connect_udp() {
  closed_udp.reset();
  pause_udp.invoke();

  // Setup variables for UDP loop
  bool is_connected = false;
  int rv;
  int byte_count;
  struct pollfd fds[1];
  fds[0].fd = socketfd_udp;
  fds[0].events = POLLIN;
  uint8_t send_buffer[] = {'P', 'I', 'N', 'G'};
  uint8_t read_buffer[8];

  /*
   * Timeout = -min(p) - min(D1) + heartbeat_period + max(D1) + max(p) 
   * p is processing time
   * delta: the time to receive the python's ping
   * heartbeat_period is the period at which this happens
   */
  int heartbeat_period  = 0;  // milliseconds. Period that Python sends pings 
  int max_delta         = 0;  // milliseconds. Max one way trip time, Python --to-> Pod
  int min_delta         = 0;  // milliseconds. Min one way trip time, Python --to-> Pod
  int max_p             = 0;  // milliseconds. Max processing time on Pod
  int min_p             = 0;  // milliseconds. Min processing time on Pod
  // Grab all configuration variables
  if (!(ConfiguratorManager::config.getValue("udp_heartbeat_period", heartbeat_period) && 
      ConfiguratorManager::config.getValue("udp_d1_max", max_delta) &&
      ConfiguratorManager::config.getValue("udp_d1_min", min_delta) &&
      ConfiguratorManager::config.getValue("udp_p_max", max_p) &&
      ConfiguratorManager::config.getValue("udp_p_min", min_p))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR -UDP- Missing necessary configuration\n");
    exit(1);  // Crash hard on this error
  }
  int connected_timeout = heartbeat_period + max_delta + max_p - min_p - min_delta;
  int timeout = -1; 
  
  udp_running.store(true);
  print(LogLevel::LOG_DEBUG, "Sim - UDP Setup complete\n");

  // Initiate the heartbeat
  udp_send(send_buffer, sizeof(send_buffer));

  // Poll indefinitely until a ping is received, then go into ping-ack loop.
  while (udp_running) {
    rv = poll(fds, 1, timeout);  // http://beej.us/guide/bgnet/html/single/bgnet.html#indexId434909-276
    if (rv == -1) {  // ERROR occured in poll()
      print(LogLevel::LOG_DEBUG, "Sim - UDP poll() failed: %s\n", strerror(errno));
      is_connected = false;
    } else if (rv == 0) {  // Timeout occured 
      print(LogLevel::LOG_DEBUG, "Sim - UDP timeout\n");
      is_connected = false;
    } else {
      if (fds[0].revents & POLLIN) {  // There is data to be read from UDP
        timeout = connected_timeout;  // Set timeout to appropriate value
        byte_count = udp_recv(read_buffer, sizeof(read_buffer));  // Read message
        if (byte_count > 0 && udp_parse(read_buffer, byte_count)) {  // Check if PING. Returns true if message was PING
          pause_udp.wait();  //  allows us to "pause" udp to simulate error
          byte_count = udp_send(send_buffer, sizeof(send_buffer));  // Respond with ACK
          if (!is_connected) {
            connected_udp.invoke();
            is_connected = true;
          }
        }
      } else {
        // print(LogLevel::LOG_ERROR, "UDP poll event, but not on specified socket with specified event\n");
        // TODO: Once Unified Command Queue is implemented, consider this as a failure & write to queue
      }
    }
  }
  closed_udp.invoke();    // tell the disconnect_udp() function we are done
}

void Simulator::disconnect_udp() {
  udp_running.store(false);
  shutdown(socketfd_udp, SHUT_RDWR);
  pause_udp.invoke();  // Trigger this just in case UDP was disabled
  closed_udp.wait();    // wait for udp loop to close
  close(socketfd_udp);  // close TCP connection
  freeaddrinfo(sendinfo_udp);  // Free memory
}


//
//
//
// CONTROL
//
//
//

void Simulator::stop() {
  logging(false);
  disconnect_tcp();
  disconnect_udp();
  std::lock_guard<std::mutex> guard(mutex);
  scenario = nullptr;
}

void Simulator::disable_tcp() {
  print(LogLevel::LOG_DEBUG, "Sim - Disable TCP\n");
  pause_tcp.reset();  // Make sure that the tcp connection loop pauses

  active_connection.store(false);  // stop the read loop
  shutdown(clientfd_tcp, SHUT_RDWR);  // shutdown client
  closed_tcp.wait();    // wait for sim_connect_tcp() to close, which was waiting on the read_loop_tcp
  close(clientfd_tcp);  // close TCP connection
  print(LogLevel::LOG_DEBUG, "Sim - Disable TCP Finished\n");
}

void Simulator::enable_tcp() {
  print(LogLevel::LOG_DEBUG, "Sim - Enable TCP\n");
  closed_tcp.reset();  // Reset this event, it is used later during shutdown
  pause_tcp.invoke();  // un-pause the TCP loop
}

void Simulator::disable_udp() {
  print(LogLevel::LOG_DEBUG, "Sim - Disable UDP\n");
  // This causes the `pause_udp.wait()` to actually wait.
  pause_udp.reset(); 
}

void Simulator::enable_udp() {
  print(LogLevel::LOG_DEBUG, "Sim - Enable UDP\n");
  // This causes the pause_udp.wait() to no longer wait.
  pause_udp.invoke();
}

//
//
//
// SCENARIOS & HOOKS
//
//
//

void Simulator::set_scenario(std::shared_ptr<Scenario> s) {
  std::lock_guard<std::mutex> guard(mutex);
  scenario = s;
}

void Simulator::logging(bool enable) {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr) {
    scenario->logging(enable);
  }
}

void Simulator::sim_relay_state(HV_Relay_Select relay, HV_Relay_State state) {
  std::lock_guard<std::mutex> guard(mutex);
  print(LogLevel::LOG_DEBUG, "Sim - Relay %d %s\n", relay, state?"Enabled":"Disabled");
  if (scenario != nullptr) {
    scenario->sim_relay_state(relay, state);
  }
}

void Simulator::sim_motor_state(bool enable) {
  std::lock_guard<std::mutex> guard(mutex);
  if (enable) {
    print(LogLevel::LOG_DEBUG, "Sim - Motors Enabled\n");
  } else {
    print(LogLevel::LOG_DEBUG, "Sim - Motors Disabled\n");
  }
  if (scenario != nullptr) {
    scenario->sim_motor_state(enable);
  }
}

void Simulator::sim_motor_set_throttle(int16_t value) {
  std::lock_guard<std::mutex> guard(mutex);
  print(LogLevel::LOG_DEBUG, "Sim - Setting motor throttle: %d\n", value);
  if (scenario != nullptr) {
    scenario->sim_motor_set_throttle(value);
  }
}

void Simulator::sim_brake_enable() {
  std::lock_guard<std::mutex> guard(mutex);
  print(LogLevel::LOG_DEBUG, "Sim - Brakes Enabled\n");
  if (scenario != nullptr) {
    scenario->sim_brake_enable();
  }
}

void Simulator::sim_brake_disable() {
  std::lock_guard<std::mutex> guard(mutex);
  print(LogLevel::LOG_DEBUG, "Sim - Brakes Disabled\n");
  if (scenario != nullptr) {
    scenario->sim_brake_disable();
  }
}

void Simulator::sim_brake_set_pressure(uint8_t value) {
  std::lock_guard<std::mutex> guard(mutex);
  print(LogLevel::LOG_DEBUG, "Setting brake pressure: %d\n", value);
  if (scenario != nullptr) {
    scenario->sim_brake_set_pressure(value);
  }
}

std::shared_ptr<ADCData> Simulator::sim_get_adc() {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr && (!scenario->use_sensor_free_motion())) {
    return scenario->sim_get_adc();
  } else {
    std::shared_ptr<ADCData> d = std::make_shared<ADCData>();
    memset(d.get(), (uint8_t)0, sizeof(ADCData));
    return d;
  }
}

std::shared_ptr<CANData> Simulator::sim_get_can() {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr && (!scenario->use_sensor_free_motion())) {
    return scenario->sim_get_can();
  } else {
    std::shared_ptr<CANData> d = std::make_shared<CANData>();
    memset(d.get(), (uint8_t)0, sizeof(CANData));
    return d;
  }
}

std::shared_ptr<I2CData> Simulator::sim_get_i2c() {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr && (!scenario->use_sensor_free_motion())) {
    return scenario->sim_get_i2c();
  } else {
    std::shared_ptr<I2CData> d = std::make_shared<I2CData>();
    memset(d.get(), (uint8_t)0, sizeof(I2CData));
    return d;
  }
}

std::shared_ptr<PRUData> Simulator::sim_get_pru() {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr && (!scenario->use_sensor_free_motion())) {
    return scenario->sim_get_pru();
  } else {
    std::shared_ptr<PRUData> d = std::make_shared<PRUData>();
    memset(d.get(), (uint8_t)0, sizeof(PRUData));
    return d;
  }
}

std::shared_ptr<MotionData> Simulator::sim_get_motion(MotionModel * mm, UnifiedState * state) {
  std::lock_guard<std::mutex> guard(mutex);
  // Either go to the scenario for motion data
  if (scenario != nullptr && (scenario->use_sensor_free_motion())) {
    return scenario->sim_get_motion();
  } else {
    // Or just call the MotionModel's usual calculate() function
    mm->calculate(state);
    return state->motion_data;
  }
}

#endif
