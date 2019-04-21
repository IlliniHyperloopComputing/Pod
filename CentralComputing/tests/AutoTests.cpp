#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"

TEST_F(PodTest, AutomaticTransitionBasic) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());

  pod->processing_command.reset();
  pod->state_machine->auto_transition_coast.wait();
  pod->processing_command.wait();

  // There is a EDGE CASE where in the time since we entered Launch/acceleration, that we have already transitioned through coast and into brake
  // such that the following line failes.
  // Then when pod->process_command.wait() happens, it waits forever since that command will never happen.
  // Thus why we have this weird if statement here.
  if(pod->state_machine->get_current_state() == E_States::ST_FLIGHT_BRAKE){
    EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
    return;
  }
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_COAST);


  pod->processing_command.reset();
  pod->state_machine->auto_transition_brake.wait();
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);

}
#endif
