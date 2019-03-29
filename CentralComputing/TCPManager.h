#ifndef TCPMANAGER_H_
#define TCPMANAGER_H_

#include "Utils.h"
#include "SafeQueue.hpp"
#include "Event.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>
#include <atomic>
#include <thread> // NOLINT
#include <memory>
#include <sys/ioctl.h>

#define SETUP_FAILURE -1
#define SETUP_SUCCESS 0

#define WRITE_FAILURE -1

namespace TCPManager {

enum Network_Command_ID {
  // state transitions
  TRANS_SAFE_MODE = 0,
  TRANS_FUNCTIONAL_TEST = 1,
  TRANS_LOADING = 2,
  TRANS_LAUNCH_READY = 3,
  LAUNCH = 4,
  EMERGENCY_BRAKE = 5,
  ENABLE_MOTOR = 6,
  DISABLE_MOTOR = 7,
  SET_MOTOR_SPEED = 8,
  ENABLE_BRAKE = 9,
  DISABLE_BRAKE = 10,
  TRANS_FLIGHT_COAST = 11,
  TRANS_FLIGHT_BRAKE = 12,
  TRANS_ERROR_STATE = 13,
};

// enum specifying what data is sent
// [1 byte Data ID][4 byte size][size byte chunk]
enum Network_Data_ID {
  POD_STATE,
  BRAKE_STATUS,
  MOTOR_STATUS,
  POSITION,
  VELOCITY,
  ACCELERATION,
  TEMPERATURE
};

/**
* A network command is returned by read and parsed within Pod.cpp
**/
struct Network_Command {
  // state transtitions
  uint8_t id; // id is just a network command
  uint8_t value;
};


extern int socketfd;

extern std::atomic<bool> running;
extern SafeQueue<std::shared_ptr<TCPManager::Network_Command>> command_queue;

extern Event connected; // Used within Simulator to check when TCP is connected
extern Event closing;   // Used to wait between writes in the write_loop()

int connect_to_server(const char * hostname, const char * port);

/**
 * Reads from socketfd, parses read bytes into a Network_Command struct
 * Note: blocking command, will wait on read until something is sent or FD is closed
 * @param buffer a pointer to the network command that was read
 * @return the number of bytes read
 **/
int read_command(Network_Command * buffer);

/** 
 * Collects data from sensor, writes to socket
 * @return bytes written or -1 if failed
 **/
int write_data();

/**
 * Thread function, continually reads commands from the socket and 
 * pushes them onto the queue
 */
void read_loop();

/**
 * Thread function, continually gets most recent state/motor/brake/sensor 
 * data and sends a packet
 */
void write_loop();

/**
 * Thread function, handles connecting to new clients and starting read/write loop
 * @param hostname the IP address to connect to
 * @param port the port to connect to
 **/
void tcp_loop(const char * hostname, const char * port);

/**
 * Closes the socket, ending all transmission
 **/
void close_client();

/*
 * Stops threads and exits
 */
void stop_threads();

} // namespace TCPManager

#endif // TCPMANAGER_H_
