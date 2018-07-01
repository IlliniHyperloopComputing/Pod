#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Utils.h"
#include "SafeQueue.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <poll.h>
#include <atomic>
#include <thread>

#define SETUP_FAILURE -1
#define SETUP_SUCCESS 0

#define WRITE_FAILURE -1

using namespace std;
namespace NetworkManager {

enum Network_Command_ID {
  //state transitions
  TRANS_SAFE_MODE = 0,
  TRANS_FUNCTIONAL_TEST = 1,
  TRANS_LOADING = 2,
  TRANS_LAUNCH_READY = 3,
  LAUNCH = 4,
  EMERGENCY_BRAKE = 5,
  ENABLE_MOTOR = 6,
  DISABLE_MOTOR = 7,
  SET_MOTOR_SPEED = 8,
  ACTIVATE_BRAKE_MAGNET = 9,
  DEACTIVATE_BRAKE_MAGNET = 10 
};

/**
* A network command is returned by read and parsed within Pod.cpp
**/
struct Network_Command {
  //state transtitions
  Network_Command_ID id; //id is just a network command
  uint8_t value;
};


int socketfd;
int clientfd;
int udp_socket;

std::atomic<bool> running(false);
sockaddr_storage addr_dest = {};
SafeQueue<Network_Command *> network_queue;


/**
* Starts the TCP server
* @param hostname the IP address to connect to
* @param port the port to connect to
* @return SETUP_SUCCESS  or SETUP_FAILURE
**/
static uint8_t start_server(const char * hostname, const char * port) {
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  int enable = 1;
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    perror("setsockopt(SO_REUSEADDR) failed");
  }

  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int s = getaddrinfo(NULL, port, &hints, &result);
  if(s != 0){
    print_info("getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }
  if(bind(socketfd, result->ai_addr, result->ai_addrlen) != 0){
    perror("bind");
    exit(1);
  }
  if(listen(socketfd, 1) != 0){
    perror("listen");
    exit(1);
  }
  print_info("Server setup successfully\n");
  free(result);

  running.store(true);
  return socketfd;
}

/**
* Starts UDP server
* @param hostname the IP address
* @param port the port
* @return SETUP_SUCCESS or SETUP_FAILURE
**/
static uint8_t start_udp(const char * hostname, const char * port) {
  return 0;
}

/**
* Blocking call--waits for client to connect
* Exits if socket is closed`
* @return the clientfd or -1
**/
static int accept_client() {
  struct pollfd p;
  p.fd = socketfd;
  p.events = POLLIN;
  int ret = 0;
  print_info("Awaiting connection\n");
  while(1) {
    ret = poll(&p, 1, 1000);
    if(ret == 1) {//there's something trying to connect
      clientfd = accept(socketfd, NULL, NULL);
      print_info("Connected!\n"); 
      return clientfd;
    }
  }
  return -1;
}

/**
* Reads from socketfd, parses read bytes into a Network_Command struct
* Note: blocking command, will wait on read until something is sent or FD is closed
* @param buffer a pointer to the network command that was read
* @return the number of bytes read
**/
static int read_command(Network_Command * buffer) {
  return read(clientfd, buffer, sizeof(Network_Command));
}

/** 
* Collects data from sensor, writes to socket
* @return bytes written or -1 if failed
**/
static int write_data() {
  // TODO 
  // Write ParameterManager::GetNetworkReport, write the report to clientfd
  return -1;
}

/**
* Closes the socket, ending all transmission with the LabView
* Should be called from a signal handler
**/
static void close_server() {
  close(clientfd);
  close(socketfd);
}

/**
* Sends a UDP Datagram with sensor data
**/
static void send_packet() {

}

static void read_loop();
static void write_loop();
static void network_loop() {
  while(running){
    int fd = accept_client();
    print_info("Client fd is: %d\n", clientfd);
    if(fd > 0){
      print_info("Starting network threads\n");
      thread read_thread(read_loop);
      thread write_thread(write_loop);

      read_thread.join();
      write_thread.join();
      print_info("Client exited, looking for next client\n");

    } else {
      PRINT_ERRNO("Accept failed, abort.")
      break;
    }
  }   
}

static void read_loop() {
  bool active_connection = true;
  Network_Command buffer;
  while (running && active_connection){
    int bytes_read = read_command(&buffer);
    active_connection = bytes_read != -1;
    Network_Command * command = new Network_Command(buffer);
    network_queue.enqueue(command);
  }

  PRINT_ERRNO("Read Loop exiting.") 
}

static void write_loop() {
  bool active_connection = true;
  while(running && active_connection){
    usleep(100000); //TODO: Change to actual value at some point
    int written = write_data();
    print_debug("Written is %d\n", written);
    active_connection = written != -1;
  }

  PRINT_ERRNO("Write Loop exiting.") 
}

static void stop_threads() {
  running.store(false);
}

}

#endif

