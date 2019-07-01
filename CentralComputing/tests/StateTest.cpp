#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"


TEST_F(PodTest, ConnectDisconnect) {
}
// States
/*
 * Safe Mode        -Functional Test
 * Functional Test outside  -Loading, SafeMode
 * Loading			-LaunchReady, SafeMode
 * Functional Test inside  -Loading, SafeMode
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


// Testing access to Functional Test Outside
TEST_F(PodTest, FunctionalTestOutside) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false); //this transition should fail because we cannot skip loading
}

// Testing acccess to Loading State
TEST_F(PodTest, Loading) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
}

// Testing acccess to Functional Test Inside
TEST_F(PodTest, FunctionalTestInside) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
}

// Testing acccess to Launch Ready
TEST_F(PodTest, LaunchReady) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
}

// Testing acccess to Flight Accel
TEST_F(PodTest, FlightAccel) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());
}

// Testing access to  Flight Coast
TEST_F(PodTest, FlightCoast) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
}

// Testing access to  Flight Brake
TEST_F(PodTest, FlightBrake) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());
}

/* 
 * All tests "Aborting" from each mode
 * non flight --> abort --> .move back to safe mode
 * flight     --> abort --> move to flight-abort
 */

// Testing access from Safe Mode to Abort
TEST_F(PodTest, SafeModeBrake) {
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_SAFE_MODE , true);
}

// Testing access from Functional Test to abort
TEST_F(PodTest, FunctionalTestOutsideAbort) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_SAFE_MODE, true);
}

// Testing access from Loading Brake to abort
TEST_F(PodTest, LoadingAbort) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_SAFE_MODE, true);
}

// Testing access from Functional Test Inside to abort
TEST_F(PodTest, FunctionalTestInsideAbort) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_SAFE_MODE, true);
}

// Testing access from Launch Ready to abort
TEST_F(PodTest, LaunchReadyAbort) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_SAFE_MODE, true);
}

// Testing access from Flight Accel to abort
TEST_F(PodTest, FlightAccelAbort) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_FLIGHT_ABORT, true);
}

// Testing access from Flight Coast to abort
TEST_F(PodTest, FlightCoastAbort) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_FLIGHT_ABORT, true);
}

// Testing access from Flight Brake to abort
TEST_F(PodTest, FlightBrakeAbort) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_FLIGHT_ABORT, true);
}



/* 
 * Tests access to Safe Mode from all of the places it should be accessed by
 */

// Testing access from Functional Test Outside to Safe Mode 
TEST_F(PodTest, FunctionalTestOutsideToSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Loading to Safe Mode 
TEST_F(PodTest, LoadingToSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Functional Test Inside to Safe Mode 
TEST_F(PodTest, FunctionalTestInsideToSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Launch Ready to Safe Mode 
TEST_F(PodTest, LaunchReadyToSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Flight Brake to Safe Mode 
TEST_F(PodTest, FlightBrakeToSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}

// Testing access from Flight Abort to Safe Mode 
TEST_F(PodTest, FlightAbortToSafe) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_FLIGHT_ABORT, true);
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, true);
}




/* 
 * All failed tests from each mode
 */

// Testing all failed access from Safe Mode
TEST_F(PodTest, SafeModeFailures) {
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Functional Test Outside
TEST_F(PodTest, FunctionalTestOutsideFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Loading
TEST_F(PodTest, LoadingFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from FunctionalTestInside
TEST_F(PodTest, FunctionalTestInsideFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Launch Ready
TEST_F(PodTest, LaunchReadyFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Flight Accel
TEST_F(PodTest, FlightAccelFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);

  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

// Testing all failed access from Flight Coast
TEST_F(PodTest, FlightCoastFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  
  MoveState(Command::Network_Command_ID::TRANS_SAFE_MODE, E_States::ST_SAFE_MODE, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, false);
}

// Testing all failed access from Flight Brake
TEST_F(PodTest, FlightBrakeFailures) {
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
  
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
}

//Testing all failed accesses from Flight - Abort
TEST_F(PodTest, FlightAbortFailures){
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  MoveState(Command::Network_Command_ID::TRANS_ABORT, E_States::ST_FLIGHT_ABORT, true);

  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, false);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, false);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, false);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, false);
}

#endif
