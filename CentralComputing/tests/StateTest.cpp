#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {
	
}


//FunctionalTest trues
TEST_F(PodTest, FunctionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
}
//Loading trues
TEST_F(PodTest, Loading) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
}
//LAUNCH ready trues
TEST_F(PodTest, Launch) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
 }
 //Accelerate
 TEST_F(PodTest, Accelerate) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
}


//SafeMode trues Brake
TEST_F(PodTest, SafeModeBrake) {
	MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}
//FunctionalTest trues Brake
TEST_F(PodTest, FunctionalTestBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}
//Loading trues Brake
TEST_F(PodTest, LoadingBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}
//LAUNCH ready trues Brake
TEST_F(PodTest, LaunchBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
 }
 //Accelerate Brake
 TEST_F(PodTest, AccelerateBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}


//Functional fails
TEST_F(PodTest, FunctionalFails) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
}
//Loading fails
TEST_F(PodTest, LoadingFails) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
}
//Launch Ready Fails
TEST_F(PodTest, LaunchreadyFails) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
}
//Acceleratefails
TEST_F(PodTest, AccelerateFails) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
}
//brakefails
TEST_F(PodTest, BrakeFails) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
}