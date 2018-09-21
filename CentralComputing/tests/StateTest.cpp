#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {
}


TEST_F(PodTest, FunctionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); //this transition should fail because we cannot skip loading
}

TEST_F(PodTest, Loading) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
}

TEST_F(PodTest, FlightAccel){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL,Pod_State::E_States::ST_FLIGHT_ACCEL,false);
}

TEST_F(PodTest,FlightCoast){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,false);
}

TEST_F(PodTest,FlightBrake){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE,Pod_State::E_States::ST_FLIGHT_BRAKE,false);
}
