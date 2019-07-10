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
  RELAY_PRE_CHARGE = 0,  // Pre-charge relay of the High-Voltage battery
  RELAY_LV_POLE = 1,  // Low Voltage pole of the High-Voltage battery
  RELAY_HV_POLE = 2,  // High Voltage pole of the High-Voltage battery
};

// Defines for selecting a particular relay state
enum HV_Relay_State {
  RELAY_ON = 0x01,
  RELAY_OFF = 0x00,
};

#define NUM_ADC 7
#define NUM_ACCEL 2
struct ADCData {
  int32_t data[NUM_ADC];
  // [0]  
  // [1]  
  // [2]  
  // [3]  
  // [4]  
  // [5]  
  // [6]  
};


struct CANData {
  // Motor Controller
  uint32_t status_word;                 // actually a uint16_t
  uint32_t position_val;                 // actually an int32_t
  uint32_t torque_val;                   // actually a int16_t
  uint32_t controller_temp;              // actually a uin8_t
  uint32_t motor_temp;                   // actually a uint8_t
  uint32_t dc_link_voltage;              // actually a int32_t
  uint32_t logic_power_supply_voltage;   // actually a int16_t
  uint32_t current_demand;               // actually a int16_t
  uint32_t motor_current_val;            // actually a uint8_t
  uint32_t electrical_angle;             // actually a int16_t
  uint32_t phase_a_current;              // actually a int16_t
  uint32_t phase_b_current;              // actually a int16_t

  // BMS
  uint32_t internal_relay_state;        // Used within the CANManager to set BMS relay states
  uint32_t relay_state;                 // uint16_t This should agree with the above (given a small delay)
  uint32_t rolling_counter;             // uint8_t 
  uint32_t fail_safe_state;             // uint16_t
  uint32_t pack_current;                // int16_t
  uint32_t pack_voltage_inst;           // uint16_t       
  uint32_t pack_voltage_open;           // uint16_t
  uint32_t pack_soc;                    // uint8_t
  uint32_t pack_amphours;               // uint16_t
  uint32_t pack_resistance;             // uint16_t
  uint32_t pack_dod;                    // uint8_t
  uint32_t pack_soh;                    // uint8_t
  uint32_t current_limit_status;        // uint16_t
  uint32_t max_pack_dcl;                // uint16_t
  uint32_t avg_pack_current;            // int16_t
  uint32_t highest_temp;                // uint8_t
  uint32_t highest_temp_id;             // uint8_t
  uint32_t avg_temp;                    // uint8_t
  uint32_t internal_temp;               // uint8_t
  uint32_t low_cell_voltage;             // uint16_t
  uint32_t low_cell_voltage_id;         // uint8_t
  uint32_t high_cell_voltage;           // uint16_t
  uint32_t high_cell_voltage_id;        // uint8_t
  uint32_t low_cell_internalR;          // uint16_t
  uint32_t low_cell_internalR_id;       // uint8_t
  uint32_t high_cell_internalR;         // uint16_t
  uint32_t high_cell_internalR_id;      // uint8_t
  uint32_t power_voltage_input;         // uint16_t
  uint32_t dtc_status_one;              // uint16_t
  uint32_t dtc_status_two;              // uint16_t
  uint32_t adaptive_total_cap;          // uint16_t
  uint32_t adaptive_amphours;           // uint16_t
  uint32_t adaptive_soc;                // uint8_t
};

#define NUM_TMP 16
struct I2CData {
  int16_t temp[NUM_TMP];
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
  CAN_MOTOR_CONTROLLER_HV_OVER_VOLTAGE_ERROR = 0x8,
  CAN_MOTOR_CONTROLLER_HV_UNDER_VOLTAGE_ERROR = 0x10,
  CAN_MOTOR_CONTROLLER_OVER_CURRENT = 0x20,
  CAN_MOTOR_CONTROLLER_MOTOR_OVER_TEMPERATURE = 0x40,
  CAN_MOTOR_CONTROLLER_INTERNAL_ERROR = 0x80,
  CAN_MOTOR_CONTROLLER_LV_OVER_VOLTAGE_ERROR = 0x100,
  CAN_MOTOR_CONTROLLER_LV_UNDER_VOLTAGE_ERROR = 0x200,
  CAN_MOTOR_CONTROLLER_INTERNAL_OVER_TEMPERATURE = 0x400,
  CAN_BMS_CELL_OVER_VOLTAGE = 0x800,
  CAN_BMS_CELL_UNDER_VOLTAGE = 0x1000,
  CAN_BMS_CELL_OVER_TEMP = 0x2000,
  CAN_BMS_BATTERY_OVER_VOLTAGE = 0x4000,
  CAN_BMS_BATTERY_UNDER_VOLTAGE = 0x8000,
  CAN_BMS_BATTERY_OVER_CURRENT = 0x10000,
};

enum I2CErrors {
  I2C_SETUP_FAILURE = 0x1,
  I2C_WRITE_ERROR = 0x2,
  I2C_READ_ERROR = 0x4,
  I2C_OVER_TEMP_ONE = 0x8,
  I2C_OVER_TEMP_TWO = 0x10,
  I2C_OVER_TEMP_THREE = 0x20,
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
  uint32_t error_vector[6]; 
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
