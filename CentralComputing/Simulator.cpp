#include "Simulator.hpp"

using namespace Utils;

Simulator SimulatorManager::sim;


bool Simulator::sim_connect(const char * hostname, const char * port) {
  //TODO connect to a Pod instance
  //
  reset_motion();
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
  print(LogLevel::LOG_DEBUG, "Enabling brakes\n");
  brakesOn = true;
}

void Simulator::sim_brake_disable() {
  print(LogLevel::LOG_DEBUG, "Disabling brakes\n");
  brakesOn = false;
}

void Simulator::sim_brake_set_pressure(uint8_t value) {
  print(LogLevel::LOG_DEBUG, "Setting brake pressure: %d\n", value);
  pressure = value;
}

std::shared_ptr<StateSpace> Simulator::sim_get_motion() {
  //FOR FIRST CALL
  if (timeLast == -1) {
    timeDelta = 0.000;
  } else { //SUBSEQUENT CALLS
    timeDelta = Utils::microseconds() - timeLast;
  }

  if (motorsOn) {
    acceleration = MAX_ACCEL * throttle;
  } else if (brakesOn) {
    acceleration = MAX_DECEL * pressure;
  } else {
    acceleration = 0;
  }

  //KINEMATIC PHYSICS CALCULATIONS
  velocity = lastVelocity + (acceleration * timeDelta);
  position = lastPosition + ((lastVelocity + velocity)/2 * timeDelta) + (0.5 * acceleration * timeDelta * timeDelta);

  //CREATING A STATESPACE OBJECT AND SETTING ITS ARRAY'S VALUES
  std::shared_ptr<StateSpace> space = std::make_shared<StateSpace>();
  space->x[0] = position;
  space->x[1] = velocity;
  space->x[2] = acceleration;

  //UPDATING VARIABLES
  lastPosition = position;
  lastVelocity = velocity;
  timeLast = Utils::microseconds();

  return space;
}



bool Simulator::send_command(std::shared_ptr<NetworkManager::Network_Command> command) {
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
