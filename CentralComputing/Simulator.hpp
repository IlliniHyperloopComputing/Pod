#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP


#include "NetworkManager.hpp"
#include "Pod_State.h"
using namespace std;
class Simulator {

  public:
    /**
     * Creates a command simulator object
     */
    Simulator();


    /**
     * Connects to the pod server at hostname/port
     * @param hostname the hostname to connect to
     * @param port the port to connect to
     */
    bool sim_connect(const char * hostname, const char * port);


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
     * Returns the pod's position within the simulation
     */
    uint8_t sim_get_position();


    /**
     * Sends the given command to the connected pod
     * @param command the command to send
     */
    bool send_command(shared_ptr<NetworkManager::Network_Command> command);


    /**
     * Thread function, reads continually and updates the internal simulate state variables
     */
    void read_loop();


    /**
     * Close the active connection and free any owned variables
     */
    void disconnect();



    Pod_State::E_States current_state; //current state of the pod as read by the network controller

    atomic<bool> active_connection;
    Event closed;
    thread read_thread;

    int socketfd;

    //TODO:
    //Add brake status, motor status
};
namespace SimulatorManager {
  extern Simulator sim;
}

#endif //SIMULATOR_HPP
