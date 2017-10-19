#ifndef NETWORK_H
#define NETWORK_H

#include "Sensor.h"
#include "Utils.h"
#include <sys/socket.h>
#include <sys/types.h>




#define SETUP_FAILURE -1
#define SETUP_SUCCESS 0

#define WRITE_FAILURE -1


enum Network_Command_ID {
  //state transitions 
  NONE,
	SAFETY_SETUP,
	SAFE_MODE,
	FUNCTIONAL_TEST,
	LAUNCH_READY,
	BRAKE,
	EMERGENCY_BRAKE,
  //propulsion 
  LAUNCH 
  //xmega handling
};
/*
  A network command is returned by read and parsed within Pod.cpp
 */
struct Network_Command {
  //state transtitions
  Network_Command_ID id;
  uint8_t value;
};


class Network {
   public:
      /**
      Constructor that sets up a network object
      **/
      Network(Sensor * sen);


      /**
      * Starts the TCP server
      * @param hostname the IP address to connect to
      * @param port the port to connect to
      * @return SETUP_SUCCESS  or SETUP_FAILURE
      **/
      uint8_t start_server(const char * hostname, const char * port);

      /**
      * Starts UDP server
      * @param hostname the IP address
      * @param port the port
      * @return SETUP_SUCCESS or SETUP_FAILURE
      **/
      uint8_t start_udp(const char * hostname, const char * port);

      /**
      * Blocking call--waits for client to connect
      * Exits if socket is closed`
      * @return the clientfd or -1
      */
      int accept();
 
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
      * Closes the socket, ending all transmission with the LabView
      * Should be called from a signal handler
      **/
      void close_server();

      /**
      * Sends a UDP Datagram with sensor data
      */
      void send_packet();

    private:
      #ifdef SIM
          //private simulation implementation variables and functions
      #else 
          
          Sensor * sensor;
          int socketfd;
          int clientfd;
          int udp_socket;
          sockaddr_storage addr_dest = {};


      #endif
 
};


#endif
