#ifdef SIM // Only compile if building test executable
#include "PodTest.cpp"
#include "ScenarioBasic.h"
#include "ScenarioRealNoFault.h"
#include "ScenarioRealLong.h"
#include "ScenarioTestTimeouts.h"
using std::make_shared;

TEST_F(PodTest, AutomaticTransitionBasic) {
  SimulatorManager::sim.set_scenario(make_shared<ScenarioBasic>());
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
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

}

TEST_F(PodTest, AutomaticTransitionSensors) {
  SimulatorManager::sim.set_scenario(make_shared<ScenarioRealNoFault>());
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
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

}

TEST_F(PodTest, AutomaticTransitionLong) {
  SimulatorManager::sim.set_scenario(make_shared<ScenarioRealLong>());
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
  MoveState(Command::Network_Command_ID::TRANS_LAUNCH_READY, E_States::ST_LAUNCH_READY, true);
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true);
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

}

TEST_F(PodTest, AutomaticTransitionTestTimeouts) {
  SimulatorManager::sim.set_scenario(make_shared<ScenarioTestTimeouts>());
  MoveState(Command::Network_Command_ID::TRANS_FUNCTIONAL_TEST, E_States::ST_FUNCTIONAL_TEST, true);
  MoveState(Command::Network_Command_ID::TRANS_LOADING, E_States::ST_LOADING, true);
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
  MoveState(Command::Network_Command_ID::LAUNCH, E_States::ST_FLIGHT_ACCEL, true); // launch into accel state
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
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_COAST);


  pod->processing_command.reset();
  pod->state_machine->auto_transition_brake.wait();
  EXPECT_GT(microseconds()-c_start, coast_timeout); // test to make sure that we waited the correct ammount of time
  EXPECT_LT(microseconds()-c_start, coast_timeout*2); // test to make sure that we are bounded 
  pod->processing_command.wait();
  EXPECT_EQ(pod->state_machine->get_current_state(), E_States::ST_FLIGHT_BRAKE);

}
#endif
