#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {
  //i dont know if this is right but i attempted it here
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::ConnectDisconnect, Pod_State::E_States:ConnectDisconnect, true);
}


TEST_F(PodTest, FunctionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); //this transition should fail because we cannot skip loading
}

TEST_F(PodTest, Loading) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
}

