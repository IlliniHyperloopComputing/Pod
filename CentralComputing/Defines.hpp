#ifndef DEFINES_HPP
#define DEFINES_HPP
#include <memory>

class MotionModel;

enum E_States : uint32_t {
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

// Defines for selecting a particular relay
enum HV_Relay_Select {
  RELAY_PRE_CHARGE = 1,  // Pre-charge relay of the High-Voltage battery
  RELAY_LV_POLE = 2,  // Low Voltage pole of the High-Voltage battery
  RELAY_HV_POLE = 3,  // High Voltage pole of the High-Voltage battery
};

// Defines for selecting a particular relay state
enum HV_Relay_State {
  RELAY_ON = 0x01,
  RELAY_OFF = 0x00,
};

#define NUM_ACCEL 3
struct ADCData {
  int32_t accel[NUM_ACCEL];
  int dummy_data;
};


struct CANData {
  // Motor Controller
  uint16_t status_word;
  int32_t position_val;
  int16_t torque_val;
  uint8_t controller_temp;
  uint8_t motor_temp;
  int32_t dc_link_voltage;
  int16_t logic_power_supply_voltage;
  int16_t current_demand;
  uint8_t motor_current_val;
  int16_t electrical_angle;
  int16_t phase_a_current;
  int16_t phase_b_current;

  // BMS
  uint32_t internal_relay_state;  // Used within the CANManager to set BMS relay states
  int relay_state;           // This should agree with the above (given a small delay)
  int rolling_counter;
  int fail_safe_sate;
  int peak_current;
  int pack_voltage_inst;
  int pack_voltage_open;
  int pack_soc;
  int pack_amphours;
  int pack_resistance;
  int pack_dod;
  int pack_soh;
  int current_limit_status;
  int max_pack_dcl;
  int avg_pack_current;
  int highest_temp;
  int highest_temp_id;
  int avg_temp;
  int internal_temp;
  int low_cell_voltge;
  int low_cell_voltage_id; 
  int high_cell_voltage;
  int high_cell_voltage_id;
  int low_cell_internalR;
  int low_cell_internalR_id;
  int high_cell_internalR;
  int high_cell_internalR_id;
  int power_voltage_input;
  int dtc_status_one;
  int dtc_status_two;
  int adaptive_total_cap;
  int adaptive_amphours;
  int adaptive_soc;
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
