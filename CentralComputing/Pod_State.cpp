#include "Pod_State.h"
#include "Simulation.h"

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
