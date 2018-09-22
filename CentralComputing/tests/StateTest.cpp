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
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
}

TEST_F(PodTest,FlightCoast){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_COAST, true);
}

TEST_F(PodTest,FlightBrake){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_COAST, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_BRAKE, true);

}

TEST_F(PodTest, FunctionalFail){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_COAST, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_BRAKE, false);

}

TEST_F(PodTest, LoadingFail){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_COAST, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_BRAKE, false);

}

TEST_F(PodTest, LaunchFail){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_COAST, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_BRAKE, false);

}

TEST_F(PodTest, CoastFail){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_COAST, false);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_BRAKE, false);

}

TEST_F(PodTest, BrakeFail){
MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST,Pod_State::E_States::ST_FUNCTIONAL_TEST,true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_COAST, true);
MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_FLIGHT_BRAKE, false);

}

