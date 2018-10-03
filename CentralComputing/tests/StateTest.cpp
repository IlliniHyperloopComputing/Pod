#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {
}

TEST_F(PodTest, SafeModeBrakes){
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

TEST_F(PodTest, FunctionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); //this transition should fail because we cannot skip loading
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false); //we cannot skip loading and preparing for launch
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); //can go back to safe mode
}

TEST_F(PodTest, FunctionalTestBrakes) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}


TEST_F(PodTest, Loading) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false); //we cannot skip preparing for launch
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); //can't go back to functional tests
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); //can go back to safe mode
}

TEST_F(PodTest, LoadingBrakes){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

TEST_F(PodTest, LaunchReady){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); //can't go back to functional tests
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); //can't go back to loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); //can go back to safe mode
}

TEST_F(PodTest, LaunchReadyBrakes){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);

TEST_F(PodTest, Accelerate) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); //can't go back to functional tests
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); //can't go back to loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); //can't go back to launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); //can go back to safe mode
}

TEST_F(PodTest, AccelerateBrakes) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

TEST_F(PodTest, EmergencyBrakes) {
  //afer emergency brakes the only state to return to is safe mode
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}

//Extra tests that may not be needed / not work
/*
TEST_F(PodTest, NonTransitionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::DISABLE_MOTOR, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::DEACTIVATE_BRAKE_MAGNET, Pod_State::E_States::ST_LOADING, true);
}

TEST_F(PodTest, NonTransitionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::DISABLE_MOTOR, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::DEACTIVATE_BRAKE_MAGNET, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
}
*/
