#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include "Command.h"
#include "Defines.hpp"
#include <string>

using namespace std;

//Test Setting error flags without Pod
TEST(ErrorFlagTest, ErrorFlagTestBasic) {
  microseconds();
  // Need to run this (especially if we are not the only test running)
  // To make sure that we can set error flags at will
  for (int i = 0; i < 8*6; i++) {
      Command::error_flag_timers[i] = -1000000;  // negative 1 second. 
  }
  Command::Network_Command com;

  // Try to set basic flag
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_SETUP_FAILURE);
  EXPECT_TRUE(Command::get(&com));
  EXPECT_EQ(com.id, Command::SET_ADC_ERROR);
  EXPECT_EQ(com.value, ADC_SETUP_FAILURE);

  // Cant set same error again that quickly
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_SETUP_FAILURE);
  EXPECT_FALSE(Command::get(&com));

  // Set different flag
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_READ_ERROR);
  EXPECT_TRUE(Command::get(&com));
  EXPECT_EQ(com.id, Command::SET_ADC_ERROR);
  EXPECT_EQ(com.value, ADC_READ_ERROR);

  // Set different flag
  Command::set_error_flag(Command::SET_NETWORK_ERROR, TCP_DISCONNECT_ERROR);
  EXPECT_TRUE(Command::get(&com));
  EXPECT_EQ(com.id, Command::SET_NETWORK_ERROR);
  EXPECT_EQ(com.value, TCP_DISCONNECT_ERROR);

  // Set different flag
  Command::set_error_flag(Command::SET_CAN_ERROR, CAN_MOTOR_CONTROLLER_ERROR);
  EXPECT_TRUE(Command::get(&com));
  EXPECT_EQ(com.id, Command::SET_CAN_ERROR);
  EXPECT_EQ(com.value, CAN_MOTOR_CONTROLLER_ERROR);

  // Cant set same error again that quickly
  Command::set_error_flag(Command::SET_CAN_ERROR, CAN_MOTOR_CONTROLLER_ERROR);
  EXPECT_FALSE(Command::get(&com));

}

//Test Setting error flags without Pod
TEST_F(PodTest, ErrorFlagTestWithPodUnifiedState) {
  std::shared_ptr<UnifiedState> unified_state;

  // Send error that something went wrong 
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_SETUP_FAILURE);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);

  // Send a different error
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_CAN_ERROR, CAN_MOTOR_CONTROLLER_ERROR);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR);

  // Send a different error of same type
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_CAN_ERROR, CAN_OVER_TEMPERATURE);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR | CAN_OVER_TEMPERATURE);

  // Send a different error of same type
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_CAN_ERROR, CAN_OVER_CURRENT);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR | CAN_OVER_TEMPERATURE | CAN_OVER_CURRENT);

  // Send a different error 
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_I2C_ERROR, I2C_READ_ERROR);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR | CAN_OVER_TEMPERATURE | CAN_OVER_CURRENT);
  EXPECT_EQ(unified_state->errors->error_vector[2], I2C_READ_ERROR);

  // Send a different error 
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_OTHER_ERROR, GPIO_SWITCH_ERROR);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR | CAN_OVER_TEMPERATURE | CAN_OVER_CURRENT);
  EXPECT_EQ(unified_state->errors->error_vector[2], I2C_READ_ERROR);
  EXPECT_EQ(unified_state->errors->error_vector[5], GPIO_SWITCH_ERROR);

  // Send a different error 
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_NETWORK_ERROR, UDP_DISCONNECT_ERROR);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR | CAN_OVER_TEMPERATURE | CAN_OVER_CURRENT);
  EXPECT_EQ(unified_state->errors->error_vector[2], I2C_READ_ERROR);
  EXPECT_EQ(unified_state->errors->error_vector[3], 0);
  EXPECT_EQ(unified_state->errors->error_vector[4], UDP_DISCONNECT_ERROR);
  EXPECT_EQ(unified_state->errors->error_vector[5], GPIO_SWITCH_ERROR);
}

TEST_F(PodTest, ErrorFlagTestWithPodClearFlag) {
  std::shared_ptr<UnifiedState> unified_state;

  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_SETUP_FAILURE);
  pod->processing_command.wait();

  // Send a different error
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_CAN_ERROR, CAN_MOTOR_CONTROLLER_ERROR);
  pod->processing_command.wait();

  // Send a different error of same type
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_CAN_ERROR, CAN_OVER_TEMPERATURE);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR | CAN_OVER_TEMPERATURE);

  pod->processing_command.reset();
  Command::put(Command::CLR_CAN_ERROR, CAN_OVER_TEMPERATURE);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], CAN_MOTOR_CONTROLLER_ERROR );

  pod->processing_command.reset();
  Command::put(Command::CLR_CAN_ERROR, CAN_MOTOR_CONTROLLER_ERROR);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], 0);

  // Do it again, nothing should happen
  pod->processing_command.reset();
  Command::put(Command::CLR_CAN_ERROR, CAN_MOTOR_CONTROLLER_ERROR);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
  EXPECT_EQ(unified_state->errors->error_vector[1], 0);

  // Do it again, nothing should happen
  pod->processing_command.reset();
  Command::put(Command::CLR_ADC_ERROR, ADC_SETUP_FAILURE);
  pod->processing_command.wait();
  // copy unified state and check it
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], 0);
  EXPECT_EQ(unified_state->errors->error_vector[1], 0);
}

TEST_F(PodTest, ErrorFlagTestWithPodMoveState) {
  std::shared_ptr<UnifiedState> unified_state;

  pod->processing_command.reset();
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  pod->processing_command.wait();
  // Send error that something went wrong 
  pod->processing_command.reset();
  Command::set_error_flag(Command::SET_ADC_ERROR, ADC_SETUP_FAILURE);
  pod->processing_command.wait();
  // Should be back in safe mode
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_SAFE_MODE);
  // copy unified state
  unified_state = pod->unified_state;
  EXPECT_EQ(unified_state->errors->error_vector[0], ADC_SETUP_FAILURE);
}

#endif
