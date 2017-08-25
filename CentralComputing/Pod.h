#ifndef _POD
#define _POD

#include "Sensor_Package.h"
#include "Pod_State.h"
#include <tuple>

int pod(int argc, char** argv);


std::tuple<bool, vector<Sensor_Configuration>> parse_input(int argc, char** argv);

void sensor_loop();
void network_loop();
void read_loop();
void write_loop();
void parse_command(char command);
enum Command {
	SAFETY_SETUP,
	SAFE_MODE,
	FUNCTIONAL_TEST,
	LAUNCH_READY,
	BRAKE,
	EMERGENCY_BRAKE,
	RESET_SENSORS,
	CALIBRATE_SENSORS,
  MANUAL_BRAKE,
  MANUAL_BRAKE_REVERSE
};

typedef struct datagram {
  uint8_t team_id;
  uint8_t status;
  int acceleration;
  int position;
  int velocity;
  int battery_voltage;
  int battery_current;
  int battery_temperature;
  int pod_temperature;
  uint32_t stripe_count;
  
} datagram;
static uint16_t VALID_STATE = 0b0001111111111111;
static double TUNNEL_LENGTH_M = 1219.2;


ssize_t read_all_from_socket(int socket, char * buffer, size_t count);
ssize_t write_all_to_socket(int socket, uint8_t * buffer, size_t count);
#endif
