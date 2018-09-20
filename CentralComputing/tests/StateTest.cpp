#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {
}


TEST_F(PodTest, SafeMode) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); 				//Safe mode
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);			//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); 			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST, Pod_State::E_States::ST_FLIGHT_COAST, false); 			//Flight coast
}

TEST_F(PodTest, FunctionalTest) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);				//Safe mode
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true); 			//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);					//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL, Pod_State::E_States::ST_FLIGHT_ACCEL, false);			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST, Pod_State::E_States::ST_FLIGHT_COAST, false);			//Flight coast
}

TEST_F(PodTest, Loading) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);				//Safe mode
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true); 					//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true); 			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST, Pod_State::E_States::ST_FLIGHT_COAST, false); 			//Flight coast
}

TEST_F(PodTest,	Brake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); 				//Safe mode
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); 			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true); 			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST, Pod_State::E_States::ST_FLIGHT_COAST, false); 			//Flight coast
}

TEST_F(PodTest, LaunchReady) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); 				//Safe mode
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);  			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true); 			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL, Pod_State::E_States::ST_FLIGHT_ACCEL, true); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST, Pod_State::E_States::ST_FLIGHT_COAST, false); 			//Flight coast
}

TEST_F(PodTest, Accelerate) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); 				//Safe mode
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);  			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true); 			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL, Pod_State::E_States::ST_FLIGHT_ACCEL, true); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST, Pod_State::E_States::ST_FLIGHT_COAST, false); 			//Flight coast
}

TEST_F(PodTest, Coast) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true); 				//Safe mode
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);  			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true); 			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_ACCEL, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST, Pod_State::E_States::ST_FLIGHT_COAST, true); 			//Flight coast
}


