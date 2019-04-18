#ifndef TCPMANAGER_H_
#define TCPMANAGER_H_

#include "Defines.hpp"
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
#include <mutex> // NOLINT
#include <sys/ioctl.h>

#define SETUP_FAILURE -1
#define SETUP_SUCCESS 0

#define WRITE_FAILURE -1

namespace TCPManager {

extern int socketfd;

extern std::atomic<bool> running;

extern SafeQueue<std::shared_ptr<UnifiedState>> write_queue;

extern Event connected;  // Used within Simulator to check when TCP is connected
extern Event closing;    // Used to wait between writes in the write_loop()
extern std::mutex mutex;  // Used to eliminate TSan errors

int connect_to_server(const char * hostname, const char * port);

/**
 * Reads from socketfd, parses read bytes into a Network_Command struct
 * Note: blocking command, will wait on read until something is sent or FD is closed
 * @param buffer a pointer to the network command that was read
 * @return the number of bytes read
 **/
int read_command(uint8_t * ID, uint8_t * Command); 
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

}  // namespace TCPManager

#endif  // TCPMANAGER_H_
