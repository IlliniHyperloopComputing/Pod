#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "TCPManager.h"
#include "StateSpace.hpp"

#define MAX_ACCEL 9.81
#define MAX_DECEL -9.81

class Simulator {
 public:
  Simulator();

  /**
   * Connects to the pod server at hostname/port
   * @param hostname the hostname to connect to
   * @param port the port to connect to
   */
  void sim_connect();
  
  int start_server(const char * hostname, const char * port);

  int accept_client();

  void logging(bool enable);

  /*
    * Simulates arming the motor
    */
  void sim_motor_enable();


  /*
    * Simulates disarming the motor
    */
  void sim_motor_disable();


  /*
    * Simulates setting the motor throttle to a specific value
    * param value the new throttle value
    */
  void sim_motor_set_throttle(uint8_t value);


  /*
    * Simulates enabling the brakes
    */
  void sim_brake_enable();


  /*
    * Simulates disabling the brakes
    */
  void sim_brake_disable();


  /*
    * Simulates setting the brake pressure to a specific Value
    * param value the new brake pressure
    */
  void sim_brake_set_pressure(uint8_t value);


  /*
    * Uses the current state of the brakes/motors to simulate the new position, velocity, 
    * and acceleration and returns them as a StateSpace object
    */
  std::shared_ptr<StateSpace> sim_get_motion();


  /**
   * Sends the given command to the connected pod
   * @param command the command to send
   */
  bool send_command(std::shared_ptr<TCPManager::Network_Command> command);


  /**
   * Thread function, reads continually and updates the internal simulate state variables
   */
  void read_loop();


  /**
   * Close the active connection and free any owned variables
   */
  void disconnect();

  /**
   * Resets the motion variables
   */
  void reset_motion();

  std::atomic<bool> active_connection;
  Event closed;
  Event connected;
  std::thread read_thread;
  std::mutex mutex;  // To get rid of data races when accessing motion data

  bool enable_logging = true;

  int socketfd;
  int clientfd;

  int64_t timeLast = -1;
  int64_t timeDelta = 0.000;
  bool motorsOn = false;
  bool brakesOn = false;
  uint8_t throttle = 0.000;
  uint8_t pressure = 0.000;
  double position = 0.000;
  double lastPosition = 0.000;
  double velocity = 0.000;
  double lastVelocity = 0.000;
  double acceleration = 0.000;
};
namespace SimulatorManager {
  extern Simulator sim;
}

#endif  // SIMULATOR_H_
