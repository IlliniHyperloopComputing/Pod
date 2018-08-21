#include "PodTest.cpp"

TEST_F(PodTest, MotorTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);

  //Enable motors
  MoveState(NetworkManager::Network_Command_ID::ENABLE_MOTOR, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  usleep(100000);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), true);

  //Disable motors
  MoveState(NetworkManager::Network_Command_ID::DISABLE_MOTOR, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  usleep(100000);
  EXPECT_EQ(pod->state_machine->motor.is_enabled(), false);


}
