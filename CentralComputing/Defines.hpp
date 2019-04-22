#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <memory>

class MotionModel;

enum E_States {
  ST_SAFE_MODE,
  ST_FUNCTIONAL_TEST,
  ST_LOADING,
  ST_LAUNCH_READY,
  ST_FLIGHT_ACCEL,
  ST_FLIGHT_COAST,
  ST_FLIGHT_BRAKE,
  ST_ERROR,
  ST_MAX_STATES
};

#define NUM_ACCEL 3
struct ADCData {
  int32_t accel[NUM_ACCEL];
  int dummy_data;
};

struct CANData {
  // replace with actual data structure
  int dummy_data;
  int32_t wheel_distance;
  int32_t drive_wheel_velocity;
};

struct I2CData {
  // replace with actual data structure
  int dummy_data;
};

#define NUM_ORANGE_INPUTS 2
#define NUM_WHEEL_INPUTS 2
struct PRUData {
  int32_t orange_distance[NUM_ORANGE_INPUTS];
  int32_t orange_velocity[NUM_ORANGE_INPUTS];
  int32_t wheel_distance[NUM_WHEEL_INPUTS];
  int32_t wheel_velocity[NUM_WHEEL_INPUTS];
};

struct MotionData {
  int32_t x[3];  // Position/Velocity/Accel
  // x[0] = x
  // x[1] = x' first derivative
  // x[2] = x'' second derivative
};

enum ADCErrors {
  ADC_SETUP_FAILURE = 0x1,
  ADC_READ_ERROR = 0x2,
};

enum CANErrors {
  CAN_SETUP_FAILURE = 0x1,
  CAN_SEND_FRAME_ERROR = 0x2,
  CAN_RECV_FRAME_ERROR = 0x4,
  CAN_OVER_VOLTAGE_ERROR = 0x8,
  CAN_UNDER_VOLTAGE_ERROR = 0x10,
  CAN_OVER_CURRENT = 0x20,
  CAN_OVER_TEMPERATURE = 0x40,
  CAN_MOTOR_CONTROLLER_ERROR = 0x80,
};

enum I2CErrors {
  I2C_SETUP_FAILURE = 0x1,
  I2C_WRITE_ERROR = 0x2,
  I2C_READ_ERROR = 0x4,
};

enum PRUErrors {
  PRU_SETUP_FAILURE = 0x1,
  PRU_WRITE_ERROR = 0x2,
  PRU_READ_ERROR = 0x4,
};

enum NETWORKErrors {
  UDP_SETUP_FAILURE = 0x1,
  TCP_SETUP_FAILURE = 0x2,
  UDP_READ_WRITE_ERROR = 0x4, 
  TCP_READ_WRITE_ERROR = 0x8, 
  UDP_DISCONNECT_ERROR = 0x10, 
  TCP_DISCONNECT_ERROR = 0x20, 
};

enum OTHERErrors {
  GPIO_SWITCH_ERROR = 0x1,
};

struct Errors{
  uint8_t error_vector[6]; 
  // [0] ADC errors
  // [1] CAN errors
  // [2] I2C errors
  // [3] PRU errors
  // [4] Networking Errors
  // [5] Other Errors
};

struct UnifiedState{
  std::shared_ptr<MotionData> motion_data;
  std::shared_ptr<ADCData> adc_data;
  std::shared_ptr<CANData> can_data;
  std::shared_ptr<I2CData> i2c_data;
  std::shared_ptr<PRUData> pru_data;
  std::shared_ptr<Errors> errors;
  E_States state;
};

#endif
