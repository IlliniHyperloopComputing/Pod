#include "Simulator.hpp"

using namespace Utils;

Simulator SimulatorManager::sim;

Simulator::Simulator() {
  //TODO set up internal variables
}

bool Simulator::sim_connect(const char * hostname, const char * port) {
  //TODO connect to a Pod instance
  struct addrinfo hints, *servinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  int rv;
  if((rv = getaddrinfo(hostname, port, &hints, &servinfo)) != 0) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error get addrinfo\n");
    return false;
  }

  if((socketfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error getting socket\n");
    return false;
  }

  if(connect(socketfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    close(socketfd);
    freeaddrinfo(servinfo);
    print(LogLevel::LOG_ERROR, "Error connecting\n");
    return false;
  }


  active_connection.store(true);

  read_thread = std::thread([&]() {
    read_loop();
  });

  freeaddrinfo(servinfo);
  return true;
}

void Simulator::sim_motor_enable() {
  print(LogLevel::LOG_DEBUG, "Enabling motors\n");
  motorsOn = true;
}

void Simulator::sim_motor_disable() {
  print(LogLevel::LOG_DEBUG, "Disabling motors\n");
  motorsOn = false;

}

void Simulator::sim_motor_set_throttle(uint8_t value) {
  print(LogLevel::LOG_DEBUG, "Setting motor throttle: %d\n", value);
  throttle = value;
}

void Simulator::sim_brake_enable() {
  // TODO simulator brake hasn't been implemented yet
  brakesOn = true;
}

void Simulator::sim_brake_disable() {
  // TODO simulator brake hasn't been implemented yet
  brakesOn = false;
}

void Simulator::sim_brake_set_pressure(uint8_t value) {
  // TODO simulator brake hasn't been implemented yet
  pressure = value;
}

std::shared_ptr<StateSpace> Simulator::sim_get_motion() {
   //use the current state of the brakes/motors to simulate the new position, velocity, acceleration and return them as a StateSpace object
  //get time from Utils.cpp
  if (timeLast == -1) {
    timeDelta = 0.000;
    timeLast = Utils::microseconds();
  }
  else {
    timeDelta = Utils::microseconds() - timeLast;
  }

  if (motorsOn) {
    acceleration = (MAX_ACCEL) * (throttle);
  } else if (brakesOn) {
    acceleration = (MAX_DECEL) * (pressure);
  } else {
    acceleration = 0;
  }

  velocity = lastVelocity + (acceleration) * (timeDelta);
  velocity = (lastVelocity + velocity)/2;
  position = lastPosition + (velocity) * (timeDelta) + (0.5) * (acceleration) * (timeDelta * timeDelta);

  shared_ptr<StateSpace> space = std::make_shared<StateSpace>();
  space -> x[0] = position;
  space -> x[1] = velocity;
  space -> x[2] = acceleration;

  lastPosition = position;
  lastVelocity = velocity;
  timeLast = Utils::microseconds();

  return space;
}



bool Simulator::send_command(shared_ptr<NetworkManager::Network_Command> command) {
  int bytes_written = write(socketfd, command.get(), sizeof(NetworkManager::Network_Command));
  //print(LogLevel::LOG_EDEBUG, "Bytes written : %d, ID : %d, Value : %d\n", bytes_written, command->id, command->value);
  int size = sizeof(NetworkManager::Network_Command);
  return bytes_written == size;

}

void Simulator::read_loop() {
  while(active_connection.load()){
    // dump the data because we don't need it or do anything with it.
    // TODO if we want to we can keep the data and use it for error checking purposes
    // but that seems redundant and like a lot of work

    char buf[100];
    read(socketfd, buf, 100);

  }
  closed.invoke();

}

void Simulator::disconnect() {
  active_connection.store(false);
  close(socketfd);
  closed.wait();
  read_thread.join();
}
