#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {
}
// States
/*
 * Safe Mode        -Functional Test
 * Functional Test  -Loading, SafeMode
 * Loading			-LaunchReady, SafeMode
 * Launch Ready		-FlightAccel, SafeMode
 * Flight Accel		-FlightCoast, FlightBrake
 * Flight Coast		-FlightBrake
 * Flight Brake		-SafeMode
 * Emergency brake moves from any state to FlightBrake
 * No function call to get to Flight Coast
 * test Brake from Emergency Brake
 MoveState(NetworkManager::Network_Command_ID::TRANS_#, Pod_State::E_States::ST_#, false);
 */

// All tests start at Safe mode

<<<<<<< HEAD
/* 
 * All tests for normal flow of the system
 */

// Testing access to Functional Test
=======
TEST_F(PodTest, SafeMode) {

  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); 			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 			//Flight accelerate
}

>>>>>>> 3bd891d97ce6e1f85ae8700d46cad0c18836989e
TEST_F(PodTest, FunctionalTest) {

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true); 			//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);			//Flight accelerate

}

// Testing acccess to Loading State
TEST_F(PodTest, Loading) {

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true); 					//Loading
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test

}

TEST_F(PodTest,	Brake) {

  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true); 			//Flight brake
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false); 			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false); 			//Flight accelerate
}

TEST_F(PodTest, LaunchReady) {

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);  			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading

}

<<<<<<< HEAD
// Testing acccess to Launch Ready
TEST_F(PodTest, LaunchReady) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
}

// Testing acccess to Flight Accel
TEST_F(PodTest, FlightAccel) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
}

// Testing access to Flight Coast
TEST_F(PodTest, Flight_Coast){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,true);
}

// Testing access to Flight Brake
TEST_F(PodTest, FlightBrake){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_BRAKE,true);
}
  

/* 
 * All tests to emergency brake from each mode
 */

// Testing access from Safe Mode to Emergency Brake
TEST_F(PodTest, SafeModeBrake) {
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Functional Test to Emergency Brake
TEST_F(PodTest, FunctionalTestBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Loading Brake to Emergency Brake
TEST_F(PodTest, LoadingBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Launch Ready to Emergency Brake
TEST_F(PodTest, LaunchReadyBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Flight Accel to Emergency Brake
TEST_F(PodTest, FlightAccelBrake) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access to Emergency brake from coast
TEST_F(PodTest, FlightCoast){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,true);
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
}
/* 
 * Tests access to Safe Mode from all of the places it should be accessed by
 */

// Testing access from Functional Test to Safe Mode 
TEST_F(PodTest, FunctionalTestSafe) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}

// Testing access from Loading to Safe Mode 
TEST_F(PodTest, LoadingSafe) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}

// Testing access from Launch Ready to Safe Mode 
TEST_F(PodTest, LaunchReadySafe) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}

// Testing access from Brake Mode to Safe Mode 
TEST_F(PodTest, BrakeSafe) {
  MoveState(NetworkManager::Network_Command_ID::EMERGENCY_BRAKE, Pod_State::E_States::ST_FLIGHT_BRAKE, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, true);
}

/* 
 * All failed tests from each mode
 */

// Testing all failed access from Safe Mode
TEST_F(PodTest, SafeModeFailures) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,false);
}

// Testing all failed access from Functional Test
TEST_F(PodTest, FunctionalTestFailures) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,false);
}

// Testing all failed access from Loading
TEST_F(PodTest, LoadingFailures) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,false);
}

// Testing all failed access from Launch Ready
TEST_F(PodTest, LaunchReadyFailures) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,false);
}

// Testing all failed access from Flight Accel
TEST_F(PodTest, FlightAccelFailures) {
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
}

// Testing all failed from Flight Coast
TEST_F(PodTest,FlightCoastFailures){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,true);

  MoveState(NetworkManager::Network_Command_ID::TRANS_SAFE_MODE, Pod_State::E_States::ST_SAFE_MODE, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,false);
  }

  //Testing all failed from Flight brake
  TEST_F(PodTest,FlightBrakeFailures){
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,true);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_BRAKE,true);
  
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, false);
  MoveState(NetworkManager::Network_Command_ID::TRANS_FLIGHT_COAST,Pod_State::E_States::ST_FLIGHT_COAST,false);

  }
=======
TEST_F(PodTest, Accelerate) {

  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, true); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, true); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, true);  			//Launch ready
  MoveState(NetworkManager::Network_Command_ID::LAUNCH, Pod_State::E_States::ST_FLIGHT_ACCEL, true); 			//Flight accelerate
  MoveState(NetworkManager::Network_Command_ID::TRANS_FUNCTIONAL_TEST, Pod_State::E_States::ST_FUNCTIONAL_TEST, false); 		//Functional test
  MoveState(NetworkManager::Network_Command_ID::TRANS_LOADING, Pod_State::E_States::ST_LOADING, false); 				//Loading
  MoveState(NetworkManager::Network_Command_ID::TRANS_LAUNCH_READY, Pod_State::E_States::ST_LAUNCH_READY, false);  			//Launch ready
}


>>>>>>> 3bd891d97ce6e1f85ae8700d46cad0c18836989e
