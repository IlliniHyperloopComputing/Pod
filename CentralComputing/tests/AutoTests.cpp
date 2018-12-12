#include "PodTest.cpp"

TEST_F(PodTest, AutomaticTransitionBasic) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());

  pod->processing_command.reset();
  pod->state_machine->auto_transition_coast.wait();
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), Pod_State::E_States::ST_FLIGHT_COAST);


  pod->processing_command.reset();
  pod->state_machine->auto_transition_brake.wait();
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), Pod_State::E_States::ST_FLIGHT_BRAKE);

}

