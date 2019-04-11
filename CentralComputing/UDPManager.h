#ifndef UDPMANAGER_H_
#define UDPMANAGER_H_

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

namespace UDPManager {

enum Connection_Status{
  NOT_YET_CONNECTED, 
  CONNECTED,
  DISCONNECTED
};

extern int socketfd;
extern std::atomic<bool> running;
extern Connection_Status connection_status;
extern struct addrinfo hints, *sendinfo, *recvinfo;

extern Event setup;
extern std::mutex mutex;

/**
 * Starts UDP server
 * @param hostname the IP address
 * @param what port are we sending to
 * @param what port are we recieving on
 * @return socket that is created
 **/
bool start_udp(const char * hostname, const char * send_port, const char * recv_port);

int udp_recv(uint8_t* recv_buf, uint8_t len);  // receives data and is put into a buffer
int udp_send(uint8_t* buf, uint8_t len);       // send data from the buffer passed in
bool udp_parse(uint8_t* buf, uint8_t len);     // parse the data in the buffer
/**
 * Uses the UDP port setup by start_udp to monitor connection status
 **/
void connection_monitor(const char * hostname, const char * send_port, const char * recv_port);

/**
 * Closes the socket, ending all transmission
 **/
void close_client();

}  // namespace UDPManager

#endif  // UDPMANAGER_H_
