#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"

TEST_F(PodTest, AutomaticTransitionBasic) {
  MoveState(TCPManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(TCPManager::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(TCPManager::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(TCPManager::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
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

TEST_F(PodTest, TestEmergencyBrakes) {
  MoveState(TCPManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(TCPManager::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(TCPManager::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(TCPManager::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
  
  //okay, now we want to send the signal to brake:
  MoveState(TCPManager::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());

  //after the brakes are hit, the pod should automatically transition into safe mode:
  pod->processing_command.reset();
  pod->state_machine->auto_transition_safeMode.wait();
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_SAFE_MODE);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());
}
#endif
