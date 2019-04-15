#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "TCPManager.h"
#include "Defines.hpp"
#include "Scenario.hpp"

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

  std::shared_ptr<ADCData> sim_get_adc();
  std::shared_ptr<I2CData> sim_get_i2c();
  std::shared_ptr<CANData> sim_get_can();
  std::shared_ptr<PRUData> sim_get_pru();

  /*
    * Uses the current state of the brakes/motors to simulate the new position, velocity, 
    * and acceleration and returns them as a MotionData object
    */
  std::shared_ptr<MotionData> sim_get_motion();

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
   * Shutdown the simulator and prepare for the next test cycle
   * Calls the disconnect() function.
   * Acts like a destructor
   */
  void stop();

  /**
   * Set the Scenario to use 
   */
  void set_scenario(std::shared_ptr<Scenario> s);

  std::atomic<bool> active_connection;
  Event closed;
  Event connected;
  std::thread read_thread;
  std::mutex mutex;  // To get rid of data races when accessing motion data

  int socketfd;
  int clientfd;

  std::shared_ptr<Scenario> scenario;

};
namespace SimulatorManager {
  extern Simulator sim;
}

#endif  // SIMULATOR_H_
