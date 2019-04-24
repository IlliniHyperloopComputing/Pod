#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "Command.h"
#include "Defines.hpp"
#include "Scenario.hpp"
#include "MotionModel.h"

/**
 * This class is designed to be the "glue" between the tests, and the rest of the codebase. There are two parts
 * 
 * Network Part:
 *   Part of this class defines and runs a TCP server that the tests use to send commands to the Pod
 * 
 * Hook Part: 
 *   The other part of this class defines hooks that the codebase calls, such that we can inject
 *   whatever sensor values we want, based on configurable "Scenarios"
 */

class Simulator {
 public:
  Simulator();

  //
  // NETWORK PART
  //

  /**
   * Set up TCP server at hostname and port. Do not connect yet
   * @param hostname the hostname to connect to
   * @param port the port to connect to
   */
  int start_server(const char * hostname, const char * port);

  /**
   * A helper function for sim_connect()
   */
  int accept_client();

  /**
   * Allows connection from a client. Spawns Read and Write threads
   */
  void sim_connect();
  
  /**
   * Sends the given command to the client (the connected pod)
   * @param command the command to send
   */
  bool send_command(std::shared_ptr<Command::Network_Command> command);

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

  //
  // HOOK PART
  //
  // NOTE: Either the ADC/I2C/CAN/PRU functions are called, or the sim_get_motion() is called
  // Motion is normally derived from the ADC/I2C/CAN/PRU values, so we could calculate it that way.
  // Or we can simply inject arbitary motion values using sim_get_motion()
  //
  // The set motor / set brake functions are not affected by the above motion model calculations

  /**
   * Set the Scenario to use 
   */
  void set_scenario(std::shared_ptr<Scenario> s);
  
  /**
   * Enables or disables logging in the simulator (printing of motion data)
   */
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
    * Simulates ADC data, gets it from scenario
    */
  std::shared_ptr<ADCData> sim_get_adc();

  /*
    * Simulates I2C data, gets it from scenario
    */
  std::shared_ptr<I2CData> sim_get_i2c();

  /*
    * Simulates CAN data, gets it from scenario
    */
  std::shared_ptr<CANData> sim_get_can();

  /*
    * Simulates PRU data, gets it from scenario
    */
  std::shared_ptr<PRUData> sim_get_pru();

  /*
    * Simulates Motion data, gets it from scenario
    * Either this is called, OR the ADC/I2C/CAN/PRU functions are called.
    * Motion is normally derived from the ADC/I2C/CAN/PRU values, so we could calculate it that way.
    * Or we can simply inject arbitary motion values (using the scenario)
    */
  std::shared_ptr<MotionData> sim_get_motion(MotionModel * mm, std::shared_ptr<UnifiedState> state);

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
}  // namespace SimulatorManager

#endif  // SIMULATOR_H_
