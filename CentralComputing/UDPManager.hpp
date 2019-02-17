#ifndef UDPMANAGER_HPP 
#define UDPMANAGER_HPP

#include "Utils.h"
#include "SafeQueue.hpp"
#include "Event.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>
#include <atomic>
#include <thread>
#include <memory>
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

/**
 * Starts UDP server
 * @param hostname the IP address
 * @param port the port
 * @return socket that is created
 **/
int start_udp(const char * hostname, const char * port);

/**
 * Uses the UDP port setup by start_udp to monitor connection status
 **/
void connection_monitor();

/**
 * Closes the socket, ending all transmission
 **/
void close_client();

/*
 * Stops threads and exits
 */
void stop_threads();

}

#endif
