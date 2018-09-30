#include "PodTest.cpp"

 
 
 TEST_F(PodTest, ConnectDisconnect) {
 }
 
 TEST_F(PodTest, SafeMode){
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
}
 TEST_F(PodTest, FunctionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true); 
   MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); 
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
   MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}
 TEST_F(PodTest, Loading) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true); 
   MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 
   MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 
   MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
   MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}
 TEST_F(PodTest, LaunchReady){
   MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
   MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
   MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
   MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 
   MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
   MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}
 TEST_F(PodTest, EmergencyBrake){
 	MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
    MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
    MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
    MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
    MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
    MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 
    MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
    MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
    MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
    MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
} 
