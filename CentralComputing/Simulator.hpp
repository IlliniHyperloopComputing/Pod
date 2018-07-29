#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP


#include "NetworkManager.hpp"
#include "Pod_State.h"
using namespace std;

class Simulator {
 
  public:
    /**
     * Creates a command simulator object
     * @param logfile filepath where pod network output will be sent to or dev/null if null
     */
    Simulator(const char * logfile);


    /**
     * Connects to the pod server at hostname/port
     * @param hostname the hostname to connect to
     * @param port the port to connect to
     */
    bool sim_connect(const char * hostname, const char * port);

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


    const char * logpath; //logpath file

    Pod_State::E_States current_state; //current state of the pod as read by the network controller

    atomic<bool> active_connection;
    thread read_thread;

    int socketfd;

    //TODO:
    //Add brake status, motor status
};

#endif //SIMULATOR_HPP
