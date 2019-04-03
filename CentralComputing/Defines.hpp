#ifndef STRUCTS_H_
#define STRUCTS_H_

#include <memory>
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
  double accel[NUM_ACCEL];
  int dummy_data;
};

struct CANData {
  // replace with actual data structure
  int dummy_data;
};

struct I2CData {
  // replace with actual data structure
  int dummy_data;
};

#define NUM_ENC_INPUTS 4
#define NUM_MOTOR_INPUTS 4
struct PRUData {
  double encoder_distance[NUM_ENC_INPUTS];
  double encoder_velocity[NUM_ENC_INPUTS];
  double disk_RPM[NUM_MOTOR_INPUTS];
};

struct MotionData {
  int32_t x[3];  // Position/Velocity/Accel
  // x[0] = x
  // x[1] = x' first derivative
  // x[2] = x'' second derivative
};

struct UnifiedState{
  std::shared_ptr<MotionData> motion_data;
  std::shared_ptr<ADCData> adc_data;
  std::shared_ptr<CANData> can_data;
  std::shared_ptr<I2CData> i2c_data;
  std::shared_ptr<PRUData> pru_data;
  E_States state;
};

#endif
