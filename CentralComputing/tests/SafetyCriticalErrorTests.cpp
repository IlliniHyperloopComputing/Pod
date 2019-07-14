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

#endif