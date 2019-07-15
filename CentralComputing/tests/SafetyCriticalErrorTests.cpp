#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include "Command.h"
#include "Defines.hpp"
#include <string>

using namespace std;

//Test to make sure that ADC error trigger correctly
TEST_F(PodTest, SafetyCritical_ADC_Errors) {
  UnifiedState * unified_state;

  int32_t error_accel_diff;
  int32_t error_pneumatic_1_over_pressure;
  int32_t error_pneumatic_2_over_pressure;
  int32_t error_pneumatic_3_over_pressure;
  int32_t error_pneumatic_4_over_pressure;
  int32_t error_battery_box_over_pressure;
  int32_t error_battery_box_under_pressure;
  int32_t accel_diff_counter_error;
  int32_t accel_diff_counter;

  // Load the current configurations
  if (!(ConfiguratorManager::config.getValue("error_accel_diff", error_accel_diff) && 
      ConfiguratorManager::config.getValue("error_pneumatic_1_over_pressure", error_pneumatic_1_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_2_over_pressure", error_pneumatic_2_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_3_over_pressure", error_pneumatic_3_over_pressure) &&
      ConfiguratorManager::config.getValue("error_pneumatic_4_over_pressure", error_pneumatic_4_over_pressure) &&
      ConfiguratorManager::config.getValue("error_battery_box_over_pressure",  error_battery_box_over_pressure) &&
      ConfiguratorManager::config.getValue("error_battery_box_under_pressure", error_battery_box_under_pressure) &&
      ConfiguratorManager::config.getValue("accel_diff_counter_error",accel_diff_counter_error))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: ADCManager Missing necessary configuration\n");
    exit(1);
  }

  std::shared_ptr<ADCData> test_data = std::make_shared<ADCData>();

  // NO errors
  // Set all data to be OK (while it is a bit high, shouldn't trigger anything)
  test_data->data[2] = (int16_t) error_pneumatic_1_over_pressure ;
  test_data->data[3] = (int16_t) error_pneumatic_2_over_pressure ;
  test_data->data[4] = (int16_t) error_pneumatic_3_over_pressure ;
  test_data->data[5] = (int16_t) error_pneumatic_4_over_pressure ;
  test_data->data[6] = (int16_t) error_battery_box_over_pressure ;
  // Send it in to check the errors... should populate error flags
  SourceManager::ADC.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(1000000); // need to wait until we are actually 
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], 0); 
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

  // OVER errors
  // Set data to be a bit high
  test_data->data[2] = (int16_t) error_pneumatic_1_over_pressure + 1;
  test_data->data[3] = (int16_t) error_pneumatic_2_over_pressure + 1;
  test_data->data[4] = (int16_t) error_pneumatic_3_over_pressure + 1;
  test_data->data[5] = (int16_t) error_pneumatic_4_over_pressure + 1;
  test_data->data[6] = (int16_t) error_battery_box_over_pressure + 1;
  // Send it in to check the errors... should populate error flags
  SourceManager::ADC.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(500000); // need to wait until we are actually 
  // Check that we have all OVER errors
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_PNEUMATIC_OVER_PRESSURE_ERROR_1 |
                                                    ADC_PNEUMATIC_OVER_PRESSURE_ERROR_2 |
                                                    ADC_PNEUMATIC_OVER_PRESSURE_ERROR_3 |
                                                    ADC_PNEUMATIC_OVER_PRESSURE_ERROR_4 |
                                                    ADC_BATTERY_BOX_OVER_PRESSURE_ERROR);
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

  // UNDER errors
  //
  test_data->data[6] = (int16_t) error_battery_box_under_pressure-1;
  SourceManager::ADC.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // wait for all commands
  Command::wait_for_empty();
  Utils::busyWait(500000); // need to wait until we are actually 

  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_PNEUMATIC_OVER_PRESSURE_ERROR_1 |
                                                    ADC_PNEUMATIC_OVER_PRESSURE_ERROR_2 |
                                                    ADC_PNEUMATIC_OVER_PRESSURE_ERROR_3 |
                                                    ADC_PNEUMATIC_OVER_PRESSURE_ERROR_4 |
                                                    ADC_BATTERY_BOX_OVER_PRESSURE_ERROR |
                                                    ADC_BATTERY_BOX_UNDER_PRESSURE_ERROR);
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

}

//Test to make sure that ADC error trigger correctly
TEST_F(PodTest, SafetyCritical_CAN_Errors) {
  UnifiedState * unified_state;

  int32_t error_motor_ctrl_over_temp;
  int32_t error_motor_over_temp;
  int32_t error_dc_link_over_voltage; 
  int32_t error_dc_link_under_voltage; 
  int32_t error_motor_ctrl_logic_over_voltage; 
  int32_t error_motor_ctrl_logic_under_voltage; 

  int32_t error_cell_over_voltage;
  int32_t error_cell_under_voltage;
  int32_t error_cell_over_temp;
  int32_t error_battery_over_voltage;
  int32_t error_battery_under_voltage;
  int32_t error_battery_over_current;
  int32_t error_bms_logic_over_voltage; 
  int32_t error_bms_logic_under_voltage; 
  int32_t error_bms_internal_over_temp; 
  int32_t error_bms_rolling_counter_timeout;

  int32_t rolling_counter_tracker;
  int64_t rolling_counter_timer;
  int32_t rolling_counter = 0;

  // Load configurations
  if (!(ConfiguratorManager::config.getValue("error_motor_ctrl_over_temp", error_motor_ctrl_over_temp) && 
      ConfiguratorManager::config.getValue("error_motor_over_temp", error_motor_over_temp) &&
      ConfiguratorManager::config.getValue("error_dc_link_over_voltage", error_dc_link_over_voltage) &&
      ConfiguratorManager::config.getValue("error_dc_link_under_voltage", error_dc_link_under_voltage) &&
      ConfiguratorManager::config.getValue("error_motor_ctrl_logic_over_voltage",  error_motor_ctrl_logic_over_voltage) &&  // NOLINT
      ConfiguratorManager::config.getValue("error_motor_ctrl_logic_under_voltage", error_motor_ctrl_logic_under_voltage) && // NOLINT
      ConfiguratorManager::config.getValue("error_cell_over_voltage",  error_cell_over_voltage) &&
      ConfiguratorManager::config.getValue("error_cell_under_voltage", error_cell_under_voltage) &&
      ConfiguratorManager::config.getValue("error_cell_over_temp", error_cell_over_temp) &&
      ConfiguratorManager::config.getValue("error_battery_over_voltage",  error_battery_over_voltage) &&
      ConfiguratorManager::config.getValue("error_battery_under_voltage", error_battery_under_voltage) &&
      ConfiguratorManager::config.getValue("error_battery_over_current", error_battery_over_current) &&
      ConfiguratorManager::config.getValue("error_bms_rolling_counter_timeout", error_bms_rolling_counter_timeout) &&
      ConfiguratorManager::config.getValue("error_bms_internal_over_temp",  error_bms_internal_over_temp) &&  // NOLINT
      ConfiguratorManager::config.getValue("error_bms_logic_over_voltage",  error_bms_logic_over_voltage) &&  // NOLINT
      ConfiguratorManager::config.getValue("error_bms_logic_under_voltage", error_bms_logic_under_voltage))) { // NOLINT
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: CANManager Missing necessary configuration\n");
    exit(1);
  }

  std::shared_ptr<CANData> test_data = std::make_shared<CANData>();

  // NO errors
  // Set all data to be OK (while it is a bit high, shouldn't trigger anything)
  test_data->status_word = 0;
  test_data->controller_temp = 30;
  test_data->motor_temp = 30;
  test_data->dc_link_voltage = 1100;
  test_data->logic_power_supply_voltage = 16;

  test_data->pack_voltage_inst = 1100;
  test_data->pack_voltage_open = 1100;
  test_data->pack_current = 0;
  test_data->highest_temp = 30;
  test_data->avg_temp = 30;
  test_data->internal_temp = 30;
  test_data->low_cell_voltage = 37000;
  test_data->high_cell_voltage = 37000;
  test_data->power_voltage_input = 160;
  test_data->dtc_status_one = 0;
  test_data->dtc_status_two = 0;
  rolling_counter++;
  test_data->rolling_counter = rolling_counter % 255;
  // Send it in to check the errors... shouldn't populate error flags
  SourceManager::CAN.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(1000000); // need to wait until we are actually 
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[1], 0); 
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

  // This will cause the rolling error to trigger
  Utils::busyWait(error_bms_rolling_counter_timeout);

  // OVER errors
  // Set data to be a bit high
  test_data->status_word = 0x88;
  test_data->controller_temp = error_motor_ctrl_over_temp+1;
  test_data->motor_temp = error_motor_over_temp+1;
  test_data->dc_link_voltage = error_dc_link_over_voltage+1;
  test_data->logic_power_supply_voltage = error_motor_ctrl_logic_over_voltage +1;

  test_data->pack_voltage_inst = error_battery_over_voltage+1;
  test_data->pack_voltage_open = error_battery_over_voltage+1;
  test_data->pack_current = error_battery_over_current+1;
  test_data->highest_temp = error_cell_over_temp+1;
  test_data->internal_temp = error_bms_internal_over_temp+1;
  test_data->low_cell_voltage = error_cell_over_voltage+1;
  test_data->high_cell_voltage = error_cell_over_voltage+1;
  test_data->power_voltage_input = error_bms_logic_over_voltage+1;
  test_data->dtc_status_one = 1;
  test_data->dtc_status_two = 2;
  // Send it in to check the errors... should populate error flags
  SourceManager::CAN.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(1000000); // need to wait until we are actually 
  // Check that we have all OVER errors
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_HV_OVER_VOLTAGE_ERROR |
                                                    CAN_MOTOR_CONTROLLER_MOTOR_OVER_TEMPERATURE |
                                                    CAN_MOTOR_CONTROLLER_LV_OVER_VOLTAGE_ERROR |
                                                    CAN_MOTOR_CONTROLLER_INTERNAL_OVER_TEMPERATURE |
                                                    CAN_BMS_CELL_OVER_VOLTAGE |
                                                    CAN_BMS_CELL_OVER_TEMP |
                                                    CAN_BMS_BATTERY_OVER_VOLTAGE |
                                                    CAN_BMS_BATTERY_OVER_CURRENT |
                                                    CAN_BMS_LV_OVER_VOLTAGE_ERROR |
                                                    CAN_BMS_INTERNAL_OVER_TEMPERATURE |
                                                    CAN_BMS_DTC1_FAULT |
                                                    CAN_BMS_DTC2_FAULT |
                                                    CAN_MOTOR_CONTROLLER_FAULT |
                                                    CAN_MOTOR_CONTROLLER_WARN |
                                                    CAN_BMS_ROLLING_COUNTER_ERROR );
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

  // UNDER errors
  //
  test_data->dc_link_voltage = error_dc_link_under_voltage-1;
  test_data->logic_power_supply_voltage = error_motor_ctrl_logic_under_voltage -1;

  test_data->pack_voltage_inst = error_battery_under_voltage - 1;
  test_data->pack_voltage_open = error_battery_under_voltage - 1;
  test_data->low_cell_voltage = error_cell_under_voltage-1;
  test_data->high_cell_voltage = error_cell_under_voltage-1;
  test_data->power_voltage_input = error_bms_logic_under_voltage-1;
  SourceManager::CAN.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // wait for all commands
  Command::wait_for_empty();
  Utils::busyWait(1000000); // need to wait until we are actually 

  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_HV_OVER_VOLTAGE_ERROR |
                                                    CAN_MOTOR_CONTROLLER_HV_UNDER_VOLTAGE_ERROR |
                                                    CAN_MOTOR_CONTROLLER_MOTOR_OVER_TEMPERATURE |
                                                    CAN_MOTOR_CONTROLLER_LV_OVER_VOLTAGE_ERROR |
                                                    CAN_MOTOR_CONTROLLER_LV_UNDER_VOLTAGE_ERROR |
                                                    CAN_MOTOR_CONTROLLER_INTERNAL_OVER_TEMPERATURE |
                                                    CAN_BMS_CELL_OVER_VOLTAGE |
                                                    CAN_BMS_CELL_UNDER_VOLTAGE |
                                                    CAN_BMS_CELL_OVER_TEMP |
                                                    CAN_BMS_BATTERY_OVER_VOLTAGE |
                                                    CAN_BMS_BATTERY_UNDER_VOLTAGE |
                                                    CAN_BMS_BATTERY_OVER_CURRENT |
                                                    CAN_BMS_LV_OVER_VOLTAGE_ERROR |
                                                    CAN_BMS_LV_UNDER_VOLTAGE_ERROR |
                                                    CAN_BMS_INTERNAL_OVER_TEMPERATURE |
                                                    CAN_BMS_DTC1_FAULT |
                                                    CAN_BMS_DTC2_FAULT |
                                                    CAN_MOTOR_CONTROLLER_FAULT |
                                                    CAN_MOTOR_CONTROLLER_WARN |
                                                    CAN_BMS_ROLLING_COUNTER_ERROR );
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

}

// 
//
//
//
//Test to make sure that I2C error trigger correctly
TEST_F(PodTest, SafetyCritical_I2C_Errors) {
  UnifiedState * unified_state;

  int32_t error_general_1_over_temp;
  int32_t error_general_2_over_temp;
  int32_t error_general_3_over_temp;

  // Load the current configurations
  if (!(ConfiguratorManager::config.getValue("error_general_1_over_temp", error_general_1_over_temp) && 
      ConfiguratorManager::config.getValue("error_general_2_over_temp",   error_general_2_over_temp) &&
      ConfiguratorManager::config.getValue("error_general_3_over_temp",   error_general_3_over_temp))) {
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: I2CManager Missing necessary configuration\n");
    exit(1);
  }

  std::shared_ptr<I2CData> test_data = std::make_shared<I2CData>();

  // NO errors
  // Set all data to be OK (while it is a bit high, shouldn't trigger anything)
  test_data->temp[0] =0;
  // Send it in to check the errors... should populate error flags
  SourceManager::I2C.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(1000000); // need to wait until we are actually 
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[2], 0); 
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

  // OVER errors
  // Set data to be a bit high
  test_data->temp[0]  = (int16_t) error_general_1_over_temp + 1;
  test_data->temp[6]  = (int16_t) error_general_2_over_temp + 1;
  test_data->temp[15] = (int16_t) error_general_3_over_temp + 1;
  // Send it  check the errors... should populate error flags
  SourceManager::I2C.check_for_sensor_error(test_data, E_States::ST_SAFE_MODE);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(1000000); // need to wait until we are actually 
  // Check that we have all OVER errors
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[2], I2C_OVER_TEMP_ONE |
                                                    I2C_OVER_TEMP_TWO |
                                                    I2C_OVER_TEMP_THREE);
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS


}

//
//
//
//
//Test to make sure that PRU error trigger correctly
TEST_F(PodTest, SafetyCritical_PRU_Errors) {
  // Nothing to do here... nothing safety critical
}

#endif