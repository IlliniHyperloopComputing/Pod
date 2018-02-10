#include "Pod_State.h"
#include "Simulation.h"

Pod_State::Pod_State(Brake * brake, Motor * motor, Sensor * sensor)
//Pod_State::Pod_State()
  : StateMachine(ST_MAX_STATES) 
{
  transition_map[TRANS_SAFE_MODE] = &Pod_State::move_safe_mode;  
  transition_map[TRANS_FUNCTIONAL_TEST] = &Pod_State::move_functional_tests;
  transition_map[TRANS_LOADING] = &Pod_State::move_loading;
  transition_map[TRANS_LAUNCH_READY] = &Pod_State::move_launch_ready;
  transition_map[LAUNCH] = &Pod_State::accelerate;
  transition_map[EMERGENCY_BRAKE] = &Pod_State::emergency_brake;
//non state transition commands
  transition_map[ENABLE_MOTOR] = &Pod_State::move_loading;
  transition_map[DISABLE_MOTOR] = &Pod_State::no_transition;
  transition_map[SET_MOTOR_SPEED] = &Pod_State::no_transition;
  transition_map[ACTIVATE_BRAKE_MAGNET] = &Pod_State::no_transition;
  transition_map[DEACTIVATE_BRAKE_MAGNET] = &Pod_State::no_transition;

  steady_state_map[ST_SAFE_MODE] = &Pod_State::steady_safe_mode;
  steady_state_map[ST_FUNCTIONAL_TEST] = &Pod_State::steady_functional;
  steady_state_map[ST_LOADING] = &Pod_State::steady_loading;
  steady_state_map[ST_LAUNCH_READY] = &Pod_State::steady_launch_ready;
  steady_state_map[ST_FLIGHT_ACCEL] = &Pod_State::steady_flight_accelerate;
  steady_state_map[ST_FLIGHT_COAST] = &Pod_State::steady_flight_coast;
  steady_state_map[ST_FLIGHT_BRAKE] = &Pod_State::steady_flight_brake;
}
// returns the current state as a E_States enum
Pod_State::E_States Pod_State::get_current_state() {
	return (E_States)StateMachine::getCurrentState();
}


/**
 * User controlled movement events
**/


void Pod_State::move_functional_tests(){
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_FUNCTIONAL_TEST)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

void Pod_State::move_safe_mode() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

void Pod_State::move_loading() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_LOADING)	  		/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* Flight brake */
	END_TRANSITION_MAP(NULL)
}
void Pod_State::move_launch_ready() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_LAUNCH_READY)		/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

// it is important all states should move to braking when this function is called, this is for emergencies
void Pod_State::emergency_brake() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Loading */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Launch ready */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight accel */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight coast */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

/**
 * Software controlled events
 **/

void Pod_State::accelerate() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_ACCEL)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}
void Pod_State::coast() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_COAST)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

void Pod_State::brake() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe Mode */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Loading */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Launch ready */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

void Pod_State::no_transition() {
  // do nothing
}

void Pod_State::ST_Safe_Mode() {
  write_sim_command(STATE, get_current_state()); 
}

void Pod_State::ST_Functional_Test() {
  write_sim_command(STATE, get_current_state());
}
void Pod_State::ST_Loading() {
  write_sim_command(STATE, get_current_state());
}
void Pod_State::ST_Launch_Ready() {
  write_sim_command(STATE, get_current_state());
}

void Pod_State::ST_Flight_Accel() {
  write_sim_command(STATE, get_current_state());
}

void Pod_State::ST_Flight_Coast() {
  write_sim_command(STATE, get_current_state());
}

void Pod_State::ST_Flight_Brake() {
  write_sim_command(STATE, get_current_state());
}

/////////////////////////////
// STEADY STATE FUNCTIONS //
///////////////////////////
void Pod_State::steady_safe_mode(Network_Command * command) {
  //not much special stuff to do here  
  print_info("In safe mode\n");
}

void Pod_State::steady_functional(Network_Command * command) {
  //process command, let manual commands go through
  print_info("In functional test\n");

}

void Pod_State::steady_loading(Network_Command * command) {

}

void Pod_State::steady_launch_ready(Network_Command * command) {

}

void Pod_State::steady_flight_accelerate(Network_Command * command) {

}

void Pod_State::steady_flight_coast(Network_Command * command) {

}

void Pod_State::steady_flight_brake(Network_Command * command) {

}
