#ifdef SIM // Only compile if building test executable
#include "Simulator.h"

using Utils::print;
using Utils::LogLevel;

Simulator SimulatorManager::sim;

Simulator::Simulator() {
  reset_motion();
  connected.reset();
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
    if (ret == 1) { // there's something trying to connect, or we are exiting
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

bool Simulator::send_command(std::shared_ptr<TCPManager::Network_Command> command) {
  int bytes_written = write(clientfd, command.get(), sizeof(TCPManager::Network_Command));
  // print(LogLevel::LOG_EDEBUG, "Sim - Bytes written : %d, ID : %d, Value : %d  clientfd : %d\n", bytes_written, command->id, command->value, clientfd);
  int size = sizeof(TCPManager::Network_Command);
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
  active_connection.store(false); // stop the read loop

  shutdown(clientfd, SHUT_RDWR);
  close(clientfd); // close TCP connection
  shutdown(socketfd, SHUT_RDWR);
  close(socketfd); // close TCP server
  closed.wait();   // wait for sim_connect() to close, which was waiting on the read_loop
}

void Simulator::logging(bool enable) {
  enable_logging = enable;
}

void Simulator::sim_motor_enable() {
  print(LogLevel::LOG_DEBUG, "Sim - Motors Enabled\n");
  motorsOn = true;
}

void Simulator::sim_motor_disable() {
  print(LogLevel::LOG_DEBUG, "Sim - Motors Disabled\n");
  motorsOn = false;

}

void Simulator::sim_motor_set_throttle(uint8_t value) {
  print(LogLevel::LOG_DEBUG, "Sim - Setting motor throttle: %d\n", value);
  throttle = value;
}

void Simulator::sim_brake_enable() {
  print(LogLevel::LOG_DEBUG, "Sim - Brakes Enabled\n");
  brakesOn = true;
}

void Simulator::sim_brake_disable() {
  print(LogLevel::LOG_DEBUG, "Sim - Brakes Disabled\n");
  brakesOn = false;
}

void Simulator::sim_brake_set_pressure(uint8_t value) {
  print(LogLevel::LOG_DEBUG, "Setting brake pressure: %d\n", value);
  pressure = value;
}

std::shared_ptr<StateSpace> Simulator::sim_get_motion() {
  // FOR FIRST CALL
  if (timeLast == -1) {
    timeDelta = 0.000;
  } else { // SUBSEQUENT CALLS
    timeDelta = Utils::microseconds() - timeLast;
  }

  if (motorsOn) {
    acceleration = MAX_ACCEL * throttle;
  } else if (brakesOn) {
    acceleration = MAX_DECEL * pressure;
  } else {
    acceleration = 0;
  }


  double deltaSeconds = static_cast<double>(timeDelta) / 1000000.0;

  // KINEMATIC PHYSICS CALCULATIONS
  velocity = lastVelocity + (acceleration * deltaSeconds);
  position = lastPosition + ((lastVelocity + velocity)/2 * deltaSeconds) + (0.5 * acceleration * deltaSeconds * deltaSeconds);

  // CREATING A STATESPACE OBJECT AND SETTING ITS ARRAY'S VALUES
  std::shared_ptr<StateSpace> space = std::make_shared<StateSpace>();
  space->x[0] = position;
  space->x[1] = velocity;
  space->x[2] = acceleration;

  if (enable_logging) {
    print(LogLevel::LOG_DEBUG, "Motion: Position: %.2f, Velocity: %.2f, Accel = %.2f, lastPos = %.2f, lastVel = %.2f, delta = %d, timeLast=%ld, t = %ld\n",
      position, velocity, acceleration, lastPosition, lastVelocity, timeDelta, timeLast, Utils::microseconds());
  }

  // UPDATING VARIABLES
  lastPosition = position;
  lastVelocity = velocity;
  timeLast = Utils::microseconds();


  return space;
}



void Simulator::reset_motion() {

    timeLast = -1;
    timeDelta = 0.0;
    
    motorsOn = false;
    brakesOn = false;
    
    throttle = 0.0;
    pressure = 0.0;
    
    position = 0.0;
    lastPosition = 0.0;
    velocity = 0.0;
    lastVelocity = 0.0;
    acceleration = 0.0;
    timeLast = -1;

}
#endif
