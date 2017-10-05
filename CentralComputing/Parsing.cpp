#include "Parsing.h"
#include "Sensor.h"
#include <vector>
#include <iostream>

using namespace std;

std::tuple<bool, vector<Sensor_Configuration>> parse_input(int argc, char** argv) {

  vector<Sensor_Configuration> configs = vector<Sensor_Configuration>();
  
  Sensor_Configuration thermo;
  thermo.type = THERMOCOUPLE;
  thermo.simulation = 0;

  Sensor_Configuration accelx;
  accelx.type = ACCELEROMETERX;
  accelx.simulation = 0;

  Sensor_Configuration accelyz;
  accelyz.type = ACCELEROMETERYZ;
  accelyz.simulation = 0;

  Sensor_Configuration brake;
  brake.type = BRAKE_PRESSURE;
  brake.simulation = 0;

  Sensor_Configuration optical;
  optical.type = OPTICAL;
  optical.simulation = 0;

  Sensor_Configuration height;
  height.type = RIDE_HEIGHT;
  height.simulation = 0;

  Sensor_Configuration tape;
  tape.type = TAPE_COUNT;
  tape.simulation = 0;
  
  Sensor_Configuration battery;
  battery.type = BATTERY;
  battery.simulation = 0;

  Sensor_Configuration current;
  current.type = CURRENT;
  current.simulation = 0;

  Sensor_Configuration true_acceleration;
  true_acceleration.type = TRUE_ACCELERATION;
  true_acceleration.simulation = 0;

  Sensor_Configuration true_velocity;
  true_velocity.type = TRUE_VELOCITY;
  true_velocity.simulation = 0;

  Sensor_Configuration true_position;
  true_position.type = TRUE_POSITION;
  true_position.simulation = 0;

  Sensor_Configuration pull_tab;
  pull_tab.type = PULL_TAB;
  pull_tab.simulation = 0;

  size_t simulating_sensors = 0;
  int c;

  while((c = getopt(argc, argv, "a:b:c:h:i:o:t:v:y:p:"))!= -1){
    switch(c) {
      case 'a':
        accelx.simulation = atoi(optarg);
        break;
      case 'b':
        brake.simulation = atoi(optarg);
        break;
      case 'c':
        tape.simulation = atoi(optarg);
        cout << "tape sim with opt: " << tape.simulation << endl;
        break;
      case 'h':
        height.simulation = atoi(optarg);
        break;
      case 'i':
        current.simulation = atoi(optarg);
        break;
      case 'o':
        optical.simulation = atoi(optarg);
        break;
      case 't':
        thermo.simulation = atoi(optarg);
        break;
      case 'v':
        battery.simulation = atoi(optarg);
        break;
      case 'y':
        accelyz.simulation = atoi(optarg);
        break;
      case 'p':
        pull_tab.simulation = atoi(optarg);
        break;
    }
    simulating_sensors++;
  }

  configs.push_back(thermo);
  configs.push_back(accelx);
  configs.push_back(accelyz);
  configs.push_back(brake);
  configs.push_back(optical);
  configs.push_back(height);
  configs.push_back(tape);
  configs.push_back(battery);
  configs.push_back(current);
  configs.push_back(true_position);
  configs.push_back(true_velocity);
  configs.push_back(true_acceleration);
  configs.push_back(pull_tab);

  return std::make_tuple(simulating_sensors != NUM_SENSORS, configs);
}
