#ifdef SIM // Only compile if building test executable
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
 * MoveState(Command::Network_Command_ID::TRANS_#, Pod_State::E_States::ST_#, false);
 **/

// All tests start at Safe mode

/* 
 * All tests for normal flow of the system
 */
//Testing access to Error 
 TEST_F(PodTest, Error){
  	MoveState(Command::Network_Command_ID::TRANS_ERROR_STATE,E_States::ST_ERROR,true);
 }


// Testing access to Functional Test
TEST_F(PodTest, FunctionalTest) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false); //this transition should fail because we cannot skip loading
}

// Testing acccess to Loading State
TEST_F(PodTest, Loading) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
}

// Testing acccess to Launch Ready
TEST_F(PodTest, LaunchReady) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
}

// Testing acccess to Flight Accel
TEST_F(PodTest, FlightAccel) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());
}

// Testing access to  Flight Coast
TEST_F(PodTest, FlightCoast) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
}

// Testing access to  Flight Brake
TEST_F(PodTest, FlightBrake) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());
}

/* 
 * All tests to emergency brake from each mode
 */

// Testing access from Safe Mode to Emergency Brake
TEST_F(PodTest, SafeModeBrake) {
  MoveState(Command::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Functional Test to Emergency Brake
TEST_F(PodTest, FunctionalTestBrake) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Loading Brake to Emergency Brake
TEST_F(PodTest, LoadingBrake) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Launch Ready to Emergency Brake
TEST_F(PodTest, LaunchReadyBrake) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Flight Accel to Emergency Brake
TEST_F(PodTest, FlightAccelBrake) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Flight Brake to Emergency Brake
TEST_F(PodTest, FlightBrakeBrake) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  MoveState(Command::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
}

// Testing access from Safe Mode to Error
TEST_F(PodTest, SafeModeError) {
  MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);
}

// Testing access from Functional Test to Error
TEST_F(PodTest, FunctionalTestError) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);
}

// Testing access from Loading Brake to Error
TEST_F(PodTest, LoadingError) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);
}

// Testing access from Launch Ready to Error
TEST_F(PodTest, LaunchReadyError) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);
}

// Testing access from Flight Accel to Error
TEST_F(PodTest, FlightAccelError) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);
}

// Testing access from Flight Coast to Error
TEST_F(PodTest, FlightCoastError) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);
}

// Testing access from Flight Brake to Error
TEST_F(PodTest, FlightBrakeError) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);
}

/* 
 * Tests access to Safe Mode from all of the places it should be accessed by
 */

// Testing access from Functional Test to Safe Mode 
TEST_F(PodTest, FunctionalTestSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Loading to Safe Mode 
TEST_F(PodTest, LoadingSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Launch Ready to Safe Mode 
TEST_F(PodTest, LaunchReadySafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Brake Mode to Safe Mode 
TEST_F(PodTest, BrakeSafe) {
  MoveState(Command::Network_Command_ID::EMERGENCY_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

/* 
 * All failed tests from each mode
 */

// Testing all failed access from Safe Mode
TEST_F(PodTest, SafeModeFailures) {
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Functional Test
TEST_F(PodTest, FunctionalTestFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Loading
TEST_F(PodTest, LoadingFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Launch Ready
TEST_F(PodTest, LaunchReadyFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);

  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Flight Accel
TEST_F(PodTest, FlightAccelFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);

  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Flight Coast
TEST_F(PodTest, FlightCoastFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, false);
}

// Testing all failed access from Flight Brake
TEST_F(PodTest, FlightBrakeFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, false);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
}

  //Testing all failed accesses from Error State
  TEST_F(PodTest, ErrorFailures){
  	MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,true);

  	MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, false);
  	MoveState(Command::Network_Command_ID:: TRANS_ERROR_STATE, E_States::ST_ERROR,false);
  	MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, false);
  	MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  	MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  	MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, false);
  	MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  	MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
  }

#endif
