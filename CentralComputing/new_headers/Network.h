#ifndef NETWORK_H
#define NETWORK_H

#include "Sensor.h"
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;


#define SETUP_FAILURE -1
#define SETUP_SUCCESS 0

#define WRITE_FAILURE -1
/*
  A network command is returned by read and parsed within Pod.cpp
 */
struct Network_Command {
  //state transtitions
  Network_Command_ID id,
  uint8_t value
};

enum Network_Command_ID {
  //state transitions

  //propulsion 

  //braking

  //xmega handling
};

class Network {
  

   public:
      
      Network(Sensor * sensor);
      /**
      * Starts the TCP server
      * @param hostname the IP address to connect to
      * @param port the port to connect to
      * @return SETUP_SUCCESS  or SETUP_FAILURE
      **/
      uint8_t start_server(char * hostname, char * port);

      /**
      * Starts UDP server
      * @param hostname the IP address
      * @param port the port
      * @return SETUP_SUCCESS or SETUP_FAILURE
      **/
      uint8_t start_udp(char * hostname, char * port);

      /**
      * Blocking call--waits for client to connect
      * Exits if socket is closed`
      */
      void accept();
 
      /**
      * Reads from socketfd, parses read bytes into a Network_Command struct
      * @return the command struct to be parsed and sent
      **/
      Network_Command read_command(); 

      /** 
      * Collects data from sensor, writes to socket
      * @return bytes written or -1 if failed
      **/
      uint8_t write_data();

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
      Sensor * sensor;
      int socketfd;
      int clientfd;
      int udp_socket;
      sockaddr_storage addr_dest = {};
 
};


#endif
