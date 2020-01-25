#ifndef DEFINES_HPP
#define DEFINES_HPP
#include <memory>

class MotionModel;

enum E_States : uint32_t {
  ST_SAFE_MODE,
  ST_FUNCTIONAL_TEST_OUTSIDE,
  ST_LOADING,
  ST_FUNCTIONAL_TEST_INSIDE,
  ST_LAUNCH_READY,
  ST_FLIGHT_ACCEL,
  ST_FLIGHT_COAST,
  ST_FLIGHT_BRAKE,
  ST_FLIGHT_ABORT,
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
  uint32_t status_word;                  // actually a uint16_t
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
  uint32_t low_cell_voltage;            // uint16_t
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

struct BMSCellBroadcastData {
  uint8_t cell_id;
  uint16_t instant_voltage;
  uint16_t internal_resistance;
  uint16_t open_voltage;
  uint8_t checksum;
} __attribute__((__packed__));

struct BMSCells {
  BMSCellBroadcastData cell_data[30];
  uint8_t num_therms_enabled;
  uint8_t lowest_therm_value;
  uint8_t highest_therm_value;
  uint8_t highest_therm_id;
  uint8_t lowest_therm_id;
  uint8_t PADDING;
  uint8_t PADDING2;
  uint8_t PADDING3;
  int8_t therm_value[40];
  // Any additional thermistor data her
};

#define NUM_TMP 16
struct I2CData {
  int16_t temp[NUM_TMP];
  int32_t pressure_sensor;  // Units are hPa
  int32_t temp_sensor;       // Units are degrees C
};

#define NUM_ORANGE_INPUTS 2
#define NUM_WHEEL_INPUTS 2
struct PRUData {
  int32_t orange_distance[NUM_ORANGE_INPUTS];
  int32_t orange_velocity[NUM_ORANGE_INPUTS];
  int32_t wheel_distance[NUM_WHEEL_INPUTS];
  int32_t wheel_velocity[NUM_WHEEL_INPUTS];
  int32_t watchdog_hz;
};

struct MotionData {
  int32_t x[3];  // Position/Velocity/Accel
  // x[0] = x
  // x[1] = x' first derivative
  // x[2] = x'' second derivative
  int64_t p_timeout;
  int64_t a_timeout;
  int64_t c_timeout;
  int64_t b_timeout;

  int64_t p_counter;
  int64_t a_counter;
  int64_t c_counter;
  int64_t b_counter;
  
  int32_t motor_state;
  int32_t brake_state;
  int32_t motor_target_torque;
  char relay_state_buf[4];
};


enum ADCErrors {
  ADC_SETUP_FAILURE = 0x1,
  ADC_READ_ERROR = 0x2,
  ADC_ACCEL_DIFF_ERROR = 0x4,
  ADC_PNEUMATIC_OVER_PRESSURE_ERROR_1 = 0x8,
  ADC_PNEUMATIC_OVER_PRESSURE_ERROR_2 = 0x10,
  ADC_PNEUMATIC_OVER_PRESSURE_ERROR_3 = 0x20,
  ADC_PNEUMATIC_OVER_PRESSURE_ERROR_4 = 0x40,
  ADC_BATTERY_BOX_OVER_PRESSURE_ERROR = 0x80,
  ADC_BATTERY_BOX_UNDER_PRESSURE_ERROR = 0x100,
  ADC_POSITIVE_SANITY_ERROR = 0x200,
  ADC_NEGATIVE_SANITY_ERROR = 0x400,
  ADC_SENTINEL = 0x800  // Not an error, but a way to easily keep track of the number of errors
  // Update Command.cpp with additional errors, or suffer segfaults
};

enum CANErrors {
  CAN_SETUP_FAILURE = 0x1,
  CAN_SEND_FRAME_ERROR = 0x2,
  CAN_RECV_FRAME_ERROR = 0x4,
  CAN_MOTOR_CONTROLLER_HV_OVER_VOLTAGE_ERROR = 0x8,
  CAN_MOTOR_CONTROLLER_HV_UNDER_VOLTAGE_ERROR = 0x10,
  CAN_MOTOR_CONTROLLER_OVER_CURRENT = 0x20, // THIS IS NO LONGER USED,
  CAN_MOTOR_CONTROLLER_MOTOR_OVER_TEMPERATURE = 0x40,
  CAN_MOTOR_CONTROLLER_INTERNAL_ERROR = 0x80,  // SUPERSEEDED BY CONTROLLER_FAULT CONTROLLER_WARN
  CAN_MOTOR_CONTROLLER_LV_OVER_VOLTAGE_ERROR = 0x100,
  CAN_MOTOR_CONTROLLER_LV_UNDER_VOLTAGE_ERROR = 0x200,
  CAN_MOTOR_CONTROLLER_INTERNAL_OVER_TEMPERATURE = 0x400,
  CAN_BMS_CELL_OVER_VOLTAGE = 0x800,
  CAN_BMS_CELL_UNDER_VOLTAGE = 0x1000,
  CAN_BMS_CELL_OVER_TEMP = 0x2000,
  CAN_BMS_BATTERY_OVER_VOLTAGE = 0x4000,
  CAN_BMS_BATTERY_UNDER_VOLTAGE = 0x8000,
  CAN_BMS_BATTERY_OVER_CURRENT = 0x10000,
  CAN_BMS_LV_OVER_VOLTAGE_ERROR = 0x20000,
  CAN_BMS_LV_UNDER_VOLTAGE_ERROR = 0x40000,
  CAN_BMS_INTERNAL_OVER_TEMPERATURE = 0x80000,
  CAN_BMS_DTC1_FAULT = 0x100000,
  CAN_BMS_DTC2_FAULT = 0x200000,
  CAN_MOTOR_CONTROLLER_FAULT = 0x400000,
  CAN_MOTOR_CONTROLLER_WARN = 0x800000,
  CAN_BMS_ROLLING_COUNTER_ERROR = 0x1000000,
  CAN_SENTINEL = 0x2000000  // Not an error, but a way to easily keep track of the number of errors
  // Update Command.cpp with additional errors, or suffer segfaults
};

enum I2CErrors {
  I2C_SETUP_FAILURE = 0x1,
  I2C_WRITE_ERROR = 0x2,
  I2C_READ_ERROR = 0x4,
  I2C_OVER_TEMP_ONE = 0x8,
  I2C_OVER_TEMP_TWO = 0x10,
  I2C_OVER_TEMP_THREE = 0x20,
  I2C_SENTINEL = 0x40  // Not an error, but a way to easily keep track of the number of errors
  // Update Command.cpp with additional errors, or suffer segfaults
};

enum PRUErrors {
  PRU_SETUP_FAILURE = 0x1,
  PRU_WRITE_ERROR = 0x2,
  PRU_READ_ERROR = 0x4,
  PRU_ORANGE_DIFF_ERROR = 0x8,
  PRU_WHEEL_DIFF_ERROR = 0x10,
  PRU_WATCHDOG_FAIL = 0x20,
  PRU_SENTINEL = 0x40  // Not an error, but a way to easily keep track of the number of errors
  // Update Command.cpp with additional errors, or suffer segfaults
};

enum NETWORKErrors {
  UDP_SETUP_FAILURE = 0x1,
  TCP_SETUP_FAILURE = 0x2,
  UDP_READ_WRITE_ERROR = 0x4,
  TCP_READ_WRITE_ERROR = 0x8,
  UDP_DISCONNECT_ERROR = 0x10,
  TCP_DISCONNECT_ERROR = 0x20,
  NET_SENTINEL = 0x40,  // Not an error, but a way to easily keep track of the number of errors
  UDP_E_BRAKE_ERROR = 0x80,
  // Update Command.cpp with additional errors, or suffer segfaults
};

enum OTHERErrors {
  GPIO_SWITCH_ERROR = 0x1,
  GPIO_SENTINEL = 0x2  // Not an error, but a way to easily keep track of the number of errors
  // Update Command.cpp with additional errors, or suffer segfaults
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
