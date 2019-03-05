#include "PodTest.cpp"

void check_test_eq(std::string file, std::string val);

void check_test_eq(std::string file, std::string val){
  std::ifstream in(file);
  std::string line;
  std::getline(in, line);
  in.close();
  EXPECT_EQ(line, val);
}


TEST_F(PodTest, MotorTest) {
  MoveState(TCPManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);

  //Enable motors
  SendCommand(TCPManager::Network_Command_ID::ENABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);

  //Disable motors
  SendCommand(TCPManager::Network_Command_ID::DISABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);

  //Enable motors
  SendCommand(TCPManager::Network_Command_ID::ENABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);

  //Disable motors
  SendCommand(TCPManager::Network_Command_ID::DISABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);

  //Enable motors
  SendCommand(TCPManager::Network_Command_ID::ENABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);


  //Try setting motor values
  SendCommand(TCPManager::Network_Command_ID::SET_MOTOR_SPEED, 0);
  EXPECT_EQ(pod->state_machine->motor.get_throttle(), 0);

  SendCommand(TCPManager::Network_Command_ID::SET_MOTOR_SPEED, 125);
  EXPECT_EQ(pod->state_machine->motor.get_throttle(), 125);

  SendCommand(TCPManager::Network_Command_ID::SET_MOTOR_SPEED, 250);
  EXPECT_EQ(pod->state_machine->motor.get_throttle(), 250);

  //Disable again
  SendCommand(TCPManager::Network_Command_ID::DISABLE_MOTOR, 0);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);
  EXPECT_EQ(pod->state_machine->motor.get_throttle(), 0);

}
