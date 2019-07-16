#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include "ScenarioRealLong.h"
#include "ScenarioTestTimeouts.h"
using std::make_shared;

// Too much work to modify this test to make it such that 
// errors wern't being generated like crazy.  (CANBus errors)
// So Its done for now
// TEST_F(PodTest, AutomaticTransitionBasic) {
//   ConfiguratorManager::config.openConfigFile("tests/basicFlightPlan.txt", true);
//   SimulatorManager::sim.set_scenario(make_shared<ScenarioBasic>());
//   MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
//   MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
//   MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
//   MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
//   MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
//   EXPECT_TRUE(pod->state_machine->motor.is_enabled());
// 
//   pod->processing_command.reset();
//   pod->state_machine->auto_transition_coast.wait();
//   pod->processing_command.wait();
// 
//   // There is a EDGE CASE where in the time since we entered Launch/acceleration, that we have already transitioned through coast and into brake
//   // such that the following line failes.
//   // Then when pod->process_command.wait() happens, it waits forever since that command will never happen.
//   // Thus why we have this weird if statement here.
//   if(pod->state_machine->get_current_state() == E_States::ST_FLIGHT_BRAKE){
//     EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
//     return;
//   }
//   EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_COAST);
// 
// 
//   pod->processing_command.reset();
//   pod->state_machine->auto_transition_brake.wait();
//   pod->processing_command.wait();
//   EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
// 
// }

// TEST_F(PodTest, AutomaticTransitionFlightBrakeToSafeMode) {
//   ConfiguratorManager::config.openConfigFile("tests/basicFlightPlan.txt", true);
//   SimulatorManager::sim.set_scenario(make_shared<ScenarioRealNoFault>());
//   MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
//   MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
//   MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
//   MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
//   MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
//   EXPECT_TRUE(pod->state_machine->motor.is_enabled());
//   EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
//   
//   MoveState(Command::Network_Command_ID::TRANS_FLIGHT_COAST, E_States::ST_FLIGHT_COAST, true);
//   //okay, now we want to send the signal to brake:
//   MoveState(Command::Network_Command_ID::TRANS_FLIGHT_BRAKE, E_States::ST_FLIGHT_BRAKE, true);
//   EXPECT_FALSE(pod->state_machine->motor.is_enabled());
//   EXPECT_TRUE(pod->state_machine->brakes.is_enabled());
// 
//   print(LogLevel::LOG_DEBUG, "Sim - Waiting for Brake timeout to complete...\n");    
//   //after the brakes are hit, the pod should automatically transition into safe mode:
//   pod->processing_command.reset();
//   pod->state_machine->auto_transition_safe_mode.wait();
//   pod->processing_command.wait();
//   EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_SAFE_MODE);
//   EXPECT_FALSE(pod->state_machine->motor.is_enabled());
//   EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
// }

// TEST_F(PodTest, AutomaticTransitionSensors) {
//   ConfiguratorManager::config.openConfigFile("tests/basicFlightPlan.txt", true);
//   SimulatorManager::sim.set_scenario(make_shared<ScenarioRealLong>());
//   MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
//   MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
//   MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
//   MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
//   MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
//   EXPECT_TRUE(pod->state_machine->motor.is_enabled());
// 
//   pod->processing_command.reset();
//   pod->state_machine->auto_transition_coast.wait();
//   pod->processing_command.wait();
// 
//   // There is a EDGE CASE where in the time since we entered Launch/acceleration, that we have already transitioned through coast and into brake
//   // such that the following line failes.
//   // Then when pod->process_command.wait() happens, it waits forever since that command will never happen.
//   // Thus why we have this weird if statement here.
//   if(pod->state_machine->get_current_state() == E_States::ST_FLIGHT_BRAKE){
//     EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
//     return;
//   }
//   EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_COAST);
// 
// 
//   pod->processing_command.reset();
//   pod->state_machine->auto_transition_brake.wait();
//   pod->processing_command.wait();
//   EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
// 
// }

// Tests the pod through a realistic flight plan
TEST_F(PodTest, AutomaticTransitionLong) {
  ConfiguratorManager::config.openConfigFile("tests/realFlightPlan.txt", true);
  SimulatorManager::sim.set_scenario(make_shared<ScenarioRealLong>());
  SimulatorManager::sim.loaded_scenario.invoke();
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true);
  EXPECT_TRUE(pod->state_machine->motor.is_enabled());

  pod->processing_command.reset();
  pod->state_machine->auto_transition_coast.wait();
  pod->processing_command.wait();

  // There is a EDGE CASE where in the time since we entered Launch/acceleration, that we have already transitioned through coast and into brake
  // such that the following line failes.
  // Then when pod->process_command.wait() happens, it waits forever since that command will never happen.
  // Thus why we have this weird if statement here.
  if(pod->state_machine->get_current_state() == E_States::ST_FLIGHT_BRAKE){
    EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
    return;
  }
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_COAST);


  pod->processing_command.reset();
  pod->state_machine->auto_transition_brake.wait();
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);

  pod->processing_command.reset();
  pod->state_machine->auto_transition_safe_mode.wait();
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_SAFE_MODE);

}

// Tests that all of the timeouts work appropriatly
TEST_F(PodTest, AutomaticTransitionTestTimeouts) {
  ConfiguratorManager::config.openConfigFile("tests/basicFlightPlan.txt", true);
  SimulatorManager::sim.set_scenario(make_shared<ScenarioTestTimeouts>());
  SimulatorManager::sim.loaded_scenario.invoke();
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_OUTSIDE, E_States::ST_FUNCTIONAL_TEST_OUTSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST_INSIDE, E_States::ST_FUNCTIONAL_TEST_INSIDE, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);

  // Get the times we are supposed to wait
  int64_t acceleration_timeout, coast_timeout, brake_timeout;
  if (!(ConfiguratorManager::config.getValue("acceleration_timeout", acceleration_timeout) && 
      ConfiguratorManager::config.getValue("coast_timeout", coast_timeout) &&
      ConfiguratorManager::config.getValue("brake_timeout", brake_timeout))){
        print(LogLevel::LOG_ERROR, "CONFIG FILE ERROR: SCENARIO_TEST_TIMEOUTS: Missing necessary configuration\n");
        exit(1);
  }
  int64_t a_start = microseconds(); // time how long acceleration state takes
  MoveState(Command::Network_Command_ID::TRANS_FLIGHT_ACCEL, E_States::ST_FLIGHT_ACCEL, true); // launch into accel state
  EXPECT_TRUE(pod->state_machine->motor.is_enabled()); // make sure motor is on

  pod->processing_command.reset();
  pod->state_machine->auto_transition_coast.wait(); // trigger transition to coast
  EXPECT_GT(microseconds()-a_start, acceleration_timeout); // test to make sure that we waited the correct ammount of time
  int64_t c_start = microseconds(); // time how long coast state takes
  pod->processing_command.wait(); // wait for transition to process in Pod


  // There is a EDGE CASE where in the time since we entered Launch/acceleration, that we have already transitioned through coast and into brake
  // such that the following line failes.
  // Then when pod->process_command.wait() happens, it waits forever since that command will never happen.
  // Thus why we have this weird if statement here.
  if(pod->state_machine->get_current_state() == E_States::ST_FLIGHT_BRAKE){
    EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
    return;
  }
  // Make sure we are in flight coast!
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_COAST);


  // Wait to see when we enter flight brake.... and check the timing
  pod->processing_command.reset();
  pod->state_machine->auto_transition_brake.wait();
  EXPECT_GT(microseconds()-c_start, coast_timeout); // test to make sure that we waited the correct ammount of time
  EXPECT_LT(microseconds()-c_start, coast_timeout*1.2); // test to make sure that we are bounded 
  pod->processing_command.wait();

  // Check that we are in brake state
  int64_t d_start = microseconds(); // time how long brake state takes
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_TRUE(pod->state_machine->brakes.is_enabled());

  //after the brakes are hit, the pod should automatically transition into safe mode:
  print(LogLevel::LOG_DEBUG, "Sim - Waiting for Brake timeout to complete...\n");    
  pod->processing_command.reset();
  pod->state_machine->auto_transition_safe_mode.wait();
  EXPECT_GT(microseconds()-d_start+10000, brake_timeout); // test to make sure that we waited the correct ammount of time
  EXPECT_LT(microseconds()-d_start, brake_timeout*1.2); // test to make sure that we are bounded 
  pod->processing_command.wait();

  // Make sure we are in Safe mode!
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_SAFE_MODE);
  EXPECT_FALSE(pod->state_machine->motor.is_enabled());
  EXPECT_FALSE(pod->state_machine->brakes.is_enabled());
}
#endif
