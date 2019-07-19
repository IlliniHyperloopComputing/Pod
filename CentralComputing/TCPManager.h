#ifndef TCPMANAGER_H_
#define TCPMANAGER_H_

#include "Configurator.h"
#include "Defines.hpp"
#include "Utils.h"
#include "SafeQueue.hpp"
#include "SourceManager.h"
#include "Event.h"
#include "Simulator.h"
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

struct TCPSendIDs {
  uint8_t adc_id = 0;
  uint8_t can_id = 1;
  uint8_t i2c_id = 2;
  uint8_t pru_id = 3;
  uint8_t motion_id = 4;
  uint8_t error_id = 5;
  uint8_t state_id = 6;
  uint8_t bms_id = 9;
};

extern TCPSendIDs TCPID;

extern int socketfd;
extern std::atomic<bool> running;

extern UnifiedState * unified_state;
extern ADCData adc_data;
extern CANData can_data;
extern BMSCells bms_data;
extern I2CData i2c_data;
extern PRUData pru_data;
extern MotionData motion_data;
extern Errors  error_data;
extern E_States state;
extern int64_t stagger_times[4];  // Used to stagger how frequently data is sent to tcp server 
extern int64_t last_sent_times[4];   // Used to store the last time a data type was sent
extern std::mutex data_mutex;  
extern int64_t write_loop_timeout;

extern Event connected;  // Used within Simulator to check when TCP is connected
extern Event closing;    // Used to wait between writes in the write_loop()
extern std::mutex setup_shutdown_mutex;  // Used to eliminate TSan errors

int connect_to_server(const char * hostname, const char * port);

/**
 * Reads from socketfd, parses read bytes into a Network_Command struct
 * Note: blocking command, will wait on read until something is sent or FD is closed
 * @param buffer a pointer to the network command that was read
 * @return the number of bytes read
 **/
int read_command(uint32_t * ID, uint32_t * Command); 
/** 
 * Collects data from sensor, writes to socket
 * @param times timings for when to send specific data
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
void tcp_loop(const char * hostname, const char * port, UnifiedState * unified_state);

/**
 * Closes the socket, ending all transmission
 **/
void close_client();

}  // namespace TCPManager

#endif  // TCPMANAGER_H_
