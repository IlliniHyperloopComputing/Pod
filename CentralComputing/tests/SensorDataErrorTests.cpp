#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include "Command.h"
#include "Defines.hpp"
#include <string>

using namespace std;

//Test to make sure that ADC error trigger correctly
TEST_F(PodTest, SensorData_ADC_Errors) {
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
  test_data->data[0] = error_accel_diff *2;
  test_data->data[1] = error_accel_diff *2;
  test_data->data[6] = error_battery_box_under_pressure;
  // Send it in to check the errors... should populate error flags
  SourceManager::ADC.check_for_sensor_error(test_data, E_States::ST_FLIGHT_ACCEL);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(500000); // need to wait until we are actually 
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], 0); 
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

  // Generate errors
  test_data->data[0] = error_accel_diff - 1;
  test_data->data[1] = error_accel_diff *2;
  // Send it in to check the errors... should populate error flags

  for (int i = 0; i < accel_diff_counter_error+1; i++) {
    SourceManager::ADC.check_for_sensor_error(test_data, E_States::ST_FLIGHT_ACCEL);
  }
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(500000); // need to wait until we are actually 
  // Check that we have all OVER errors
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_ACCEL_DIFF_ERROR);
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS
}

//Test to make sure that ADC error trigger correctly
TEST_F(PodTest, SensorData_CAN_Errors) {
  // No Sensor data errors here
}

// 
//
//
//
//Test to make sure that I2C error trigger correctly
TEST_F(PodTest, SensorData_I2C_Errors) {
  // No Sensor data errors here
}

//
//
//
//
//Test to make sure that PRU error trigger correctly
TEST_F(PodTest, SensorData_PRU_Errors) {
  UnifiedState * unified_state;
  int32_t error_orange_diff;
  int32_t error_orange_diff_count;
  int32_t error_encoder_wheel_diff;
  int32_t error_encoder_wheel_diff_count;
  int32_t error_watchdog_heartbeat_min_hz;
  
  int32_t orange_diff_counter;
  int32_t wheel_diff_counter;
  // Nothing to do here... nothing safety critical
  if (!(ConfiguratorManager::config.getValue("error_orange_diff", error_orange_diff) && 
        ConfiguratorManager::config.getValue("error_orange_diff_count", error_orange_diff_count ) &&
        ConfiguratorManager::config.getValue("error_encoder_wheel_diff_count", error_encoder_wheel_diff_count) &&
        ConfiguratorManager::config.getValue("error_watchdog_heartbeat_min_hz", error_watchdog_heartbeat_min_hz) &&
      ConfiguratorManager::config.getValue("error_encoder_wheel_diff", error_encoder_wheel_diff))) { 
    print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: PRUManager Missing necessary configuration\n");
    exit(1);
  }

  std::shared_ptr<PRUData> test_data = std::make_shared<PRUData>();

  test_data->orange_distance[0] = error_orange_diff*2;
  test_data->orange_distance[1] = error_orange_diff*2;
  test_data->watchdog_hz = error_watchdog_heartbeat_min_hz + 1;

  test_data->wheel_distance[0] = error_encoder_wheel_diff*2;
  test_data->wheel_distance[1] = error_encoder_wheel_diff*2;

  SourceManager::PRU.check_for_sensor_error(test_data, E_States::ST_FLIGHT_ACCEL);
  // Wait for all commands to be processed.....
  Command::wait_for_empty();
  Utils::busyWait(500000); // need to wait until we are actually 
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[3], 0); 
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS

  // Generate errors
  test_data->orange_distance[0] = error_orange_diff-1;
  test_data->orange_distance[1] = error_orange_diff*2;
  test_data->wheel_distance[0] = error_encoder_wheel_diff-1;
  test_data->wheel_distance[1] = error_encoder_wheel_diff*2;
  // Send it in to check the errors... should populate error flags
  for (int i = 0; i< error_orange_diff_count+1; i++) {
    SourceManager::PRU.check_for_sensor_error(test_data, E_States::ST_FLIGHT_ACCEL);
  }

  // Cause an error
  for (int i = 0; i < error_encoder_wheel_diff_count+1; i++) {
    SourceManager::PRU.check_for_sensor_error(test_data, E_States::ST_FLIGHT_ACCEL);
  }

  Command::wait_for_empty();
  Utils::busyWait(500000); // need to wait until we are actually 
  TCPManager::data_mutex.lock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  unified_state = &pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[3], PRU_ORANGE_DIFF_ERROR |
                                                    PRU_WHEEL_DIFF_ERROR); 
  TCPManager::data_mutex.unlock();   // MUST USE LOCK TO AVOID TSAN ERRORS
  
}

#endif