#ifdef SIM  // Only compile if building test executable
#include "Simulator.h"

using Utils::print;
using Utils::LogLevel;

Simulator SimulatorManager::sim;

Simulator::Simulator() {
  connected.reset();
  scenario = nullptr;
}

int Simulator::start_server(const char * hostname, const char * port) {
  connected.reset();
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  // int blocking = 0; // 0 is blocking, 1 is non-blocking
  // ioctl(socketfd, FIONBIO, &blocking); 
  int enable = 1;
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
  }

  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int s = getaddrinfo(hostname, port, &hints, &result);
  if (s != 0) {
    print(LogLevel::LOG_ERROR, "Sim - getaddrinfo: %s\n", gai_strerror(s));
    return -1;
  }
  if (bind(socketfd, result->ai_addr, result->ai_addrlen) != 0) {
    perror("bind");
    return -1;
  }
  if (listen(socketfd, 1) != 0) {
    perror("listen");
    return -1;
  }
  print(LogLevel::LOG_DEBUG, "Sim - Server setup successfully, socketfd : %d\n", socketfd);
  free(result);

  return socketfd;
}

int Simulator::accept_client() {
  struct pollfd p;
  p.fd = socketfd;
  p.events = POLLIN;
  int ret = 0;
  print(LogLevel::LOG_DEBUG, "Sim - Awaiting connection from Pod\n");
  while (1) {
    ret = poll(&p, 1, 200);
    if (ret == 1) {  // there's something trying to connect, or we are exiting
      clientfd = accept(socketfd, NULL, NULL);
      if (clientfd != -1) {
        print(LogLevel::LOG_DEBUG, "Sim - Connected! \n"); 
      }
      return clientfd;
    }
  }
  return -1;
}

void Simulator::sim_connect() {
  closed.reset();

  // try to connect to a client
  if (accept_client() > 0) {
    print(LogLevel::LOG_DEBUG, "Sim - Starting network read thread\n");
    active_connection.store(true);
    read_thread = std::thread([&]() { read_loop(); });
    connected.invoke();
    read_thread.join();
    print(LogLevel::LOG_DEBUG, "Sim - Pod client exited. \n");
  } 
}

bool Simulator::send_command(std::shared_ptr<Command::Network_Command> command) {
  int bytes_written = write(clientfd, command.get(), sizeof(Command::Network_Command));
  // print(LogLevel::LOG_EDEBUG, "Sim - Bytes written : %d, ID : %d, Value : %d  
  //        clientfd : %d\n", bytes_written, command->id, command->value, clientfd);
  int size = sizeof(Command::Network_Command);
  return bytes_written == size;
}

void Simulator::read_loop() {
  while (active_connection.load()) {
    // dump the data because we don't need it or do anything with it.
    // TODO if we want to we can keep the data and use it for error checking purposes
    // but that seems redundant and like a lot of work

    char buf[100];
    read(clientfd, buf, 99);
  }
  closed.invoke();
}

void Simulator::disconnect() {
  active_connection.store(false);  // stop the read loop

  shutdown(clientfd, SHUT_RDWR);
  closed.wait();    // wait for sim_connect() to close, which was waiting on the read_loop
  close(clientfd);  // close TCP connection
  shutdown(socketfd, SHUT_RDWR);
  close(socketfd);  // close TCP server
}

void Simulator::stop() {
  logging(false);
  disconnect();
  std::lock_guard<std::mutex> guard(mutex);
  scenario = nullptr;
}

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

void Simulator::sim_motor_enable() {
  std::lock_guard<std::mutex> guard(mutex);
  print(LogLevel::LOG_DEBUG, "Sim - Motors Enabled\n");
  if (scenario != nullptr) {
    scenario->sim_motor_enable();
  }
}

void Simulator::sim_motor_disable() {
  std::lock_guard<std::mutex> guard(mutex);
  print(LogLevel::LOG_DEBUG, "Sim - Motors Disabled\n");
  if (scenario != nullptr) {
    scenario->sim_motor_disable();
  }
}

void Simulator::sim_motor_set_throttle(uint8_t value) {
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
  if (scenario != nullptr && (!scenario->use_motion_model())) {
    return scenario->sim_get_adc();
  } else {
    std::shared_ptr<ADCData> d = std::make_shared<ADCData>();
    memset(d.get(), (uint8_t)0, sizeof(ADCData));
    return d;
  }
}

std::shared_ptr<CANData> Simulator::sim_get_can() {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr && (!scenario->use_motion_model())) {
    return scenario->sim_get_can();
  } else {
    std::shared_ptr<CANData> d = std::make_shared<CANData>();
    memset(d.get(), (uint8_t)0, sizeof(CANData));
    return d;
  }
}

std::shared_ptr<I2CData> Simulator::sim_get_i2c() {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr && (!scenario->use_motion_model())) {
    return scenario->sim_get_i2c();
  } else {
    std::shared_ptr<I2CData> d = std::make_shared<I2CData>();
    memset(d.get(), (uint8_t)0, sizeof(I2CData));
    return d;
  }
}

std::shared_ptr<PRUData> Simulator::sim_get_pru() {
  std::lock_guard<std::mutex> guard(mutex);
  if (scenario != nullptr && (!scenario->use_motion_model())) {
    return scenario->sim_get_pru();
  } else {
    std::shared_ptr<PRUData> d = std::make_shared<PRUData>();
    memset(d.get(), (uint8_t)0, sizeof(PRUData));
    return d;
  }
}

std::shared_ptr<MotionData> Simulator::sim_get_motion(MotionModel * mm, std::shared_ptr<UnifiedState> state) {
  std::lock_guard<std::mutex> guard(mutex);
  // Either go to the scenario for motion data
  if (scenario != nullptr && (scenario->use_motion_model())) {
    return scenario->sim_get_motion();
  } else {
    // Or just call the MotionModel's usual calculate() function
    mm->calculate(state);
    return state->motion_data;
  }
}

#endif
