#ifdef SIM
#include "PodTest.cpp"
using std::make_shared;

TEST_F(PodTest, CheckMotorBrakeStates) {

  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brake.is_enabled());

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brake.is_enabled());
  
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brake.is_enabled());

  MoveState(Command::Network_Command_ID::TRANS_Functional_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true)
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brake.is_enabled());

  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::LAUNCH_READY, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brake.is_enabled());

  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brake.is_enabled());

  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brake.is_enabled());

  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brake.is_enabled());

  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_FLIGHT_ABORT, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brake.is_enabled());

}

#endif
