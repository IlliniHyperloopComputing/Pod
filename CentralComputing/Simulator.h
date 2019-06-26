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
  int start_server_tcp(const char * hostname, const char * port);

  /**
   * A helper function for sim_connect_tcp()
   */
  int accept_client_tcp();

  /**
   * Allows connection from a client. Spawns Read and Write threads
   */
  void sim_connect_tcp();
  
  /**
   * Sends the given command to the client (the connected pod)
   * @param command the command to send
   */
  bool send_command(std::shared_ptr<Command::Network_Command> command);

  /**
   * Thread function, reads continually and updates the internal simulate state variables
   */
  void read_loop_tcp();

  /**
   * Close the active tcp connection and free any owned variables
   */
  void disconnect_tcp();

  /**
   * Sets up the UDP part of the connection
   */
  bool start_udp(const char * hostname, const char * send_port, const char * recv_port);

  int udp_recv(uint8_t* recv_buf, uint8_t len);  // receives data and is put into a buffer
  int udp_send(uint8_t* buf, uint8_t len);       // send data from the buffer passed in
  bool udp_parse(uint8_t* buf, uint8_t len);     // parse the data in the buffer

  /**
   * Gets the udp heartbeat going 
   */
  void sim_connect_udp();

  /**
   * Close the active udp connection and free any owned variables
   */
  void disconnect_udp();

  /**
   * Shutdown the simulator and prepare for the next test cycle
   * Calls the disconnect() function.
   * Acts like a destructor
   */
  void stop();

  /**
   * Disable/ enable the tcp connection. This should 
   * allow us to test if the Pod can sense a disconnection and does the appropriate thing.
   * 
   * This uses some weird Event logic to pause it, hopefully it makes sense
   */
  void disable_tcp();
  void enable_tcp();

  /**
   * Only works AFTER the connection is setup
   * 
   * Disable/ enable the udp connection. This should 
   * allow us to test if the Pod can sense a disconnection and does the appropriate thing.
   * 
   * This uses some weird Event logic to pause it, hopefully it makes sense
   */
  void disable_udp();
  void enable_udp();

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
    * Simulate setting the relay state 
    */
  void sim_relay_state(HV_Relay_Select, HV_Relay_State);

  /*
    * Simulates arming the motor (true)
    * Simulates disarming the motr (false)
    */
  void sim_motor_state(bool enable);

  /*
    * Simulates setting the motor throttle to a specific value
    * param value the new throttle value
    */
  void sim_motor_set_throttle(int16_t value);

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
  std::shared_ptr<MotionData> sim_get_motion(MotionModel * mm, UnifiedState * state);

  std::atomic<bool> active_connection;
  std::atomic<bool> do_accept_client_tcp;
  std::atomic<bool> udp_running;
  Event closed_tcp;
  Event closed_udp;
  Event connected_tcp;
  Event connected_udp;
  Event pause_tcp;
  Event pause_udp;
  std::thread read_thread;
  std::mutex mutex;  // To get rid of data races when accessing motion data

  // used with udp stuff
  struct addrinfo hints_udp;
  struct addrinfo * sendinfo_udp = NULL;
  struct addrinfo * recvinfo_udp = NULL;

  int socketfd_tcp, socketfd_udp;
  int clientfd_tcp, clientfd_udp;

  std::shared_ptr<Scenario> scenario;
};

namespace SimulatorManager {
  extern Simulator sim;
}  // namespace SimulatorManager

#endif  // SIMULATOR_H_
