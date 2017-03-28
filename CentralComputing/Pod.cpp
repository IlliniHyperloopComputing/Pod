#include "Pod.h"
#include <assert.h>

// returns the current state as a E_States enum
Pod::E_States Pod::getCurrentState() {
	return (E_States)StateMachine::getCurrentState();
}

std::string Pod::get_current_state_string() {
		std::string states[] =
		{
			"SAFE_MODE",
			"FUNCTIONAL_TESTS",
			"FLIGHT_ACCEL",
			"FLIGHT_COAST",
			"FLIGHT_BRAKE",
			"NULL"
		};
		return states[(int)getCurrentState()];
}

/**
 * User controlled movement events
**/
void Pod::move_functional_tests(){
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_FUNCTIONAL_TEST)	/* Safe mode */
		TRANSITION_MAP_ENTRY(ST_FUNCTIONAL_TEST)	/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

void Pod::move_safe_mode() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Safe mode */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Functional test */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(ST_SAFE_MODE)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

void Pod::move_flight_acceleration() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Safe mode */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_ACCEL)		/* Functional test */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_ACCEL)		/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}

// it is important all states should move to braking when this function is called, this is for emergencies
void Pod::emergency_brake() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)		/* SAFE MODE */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)		/* FUNCTIONAL TEST */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)		/* FLIGHT ACCEL */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)		/* FLIGHT COAST */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)		/* FLIGHT BRAKE */
	END_TRANSITION_MAP(NULL)
}

/**
 * Software controlled events
 **/
void Pod::coast() {
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* SAFE MODE */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* FUNCTIONAL TEST */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_COAST)	/* FLIGHT ACCEL */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_COAST)	/* FLIGHT COAST */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* FLIGHT BRAKE */
	END_TRANSITION_MAP(NULL)
}

void Pod::brake() {
	BEGIN_TRANSITION_MAP						/* Current state */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* SAFE MODE */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* FUNCTIONAL TEST */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)		/* FLIGHT ACCEL */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)	/* FLIGHT COAST */
		TRANSITION_MAP_ENTRY(ST_FLIGHT_BRAKE)	/* FLIGHT BRAKE */
	END_TRANSITION_MAP(NULL)
}


// State Machine State functions
void Pod::ST_Safe_Mode() {
	std::cout << "Entering: Safe Mode" << std::endl;
	//std::cout << GetCurrentState() << std::endl;
	// TODO implement here
}

void Pod::ST_Functional_Test() {
	std::cout << "Entering: Functional Test" << std::endl;
	// TODO implement here
}

void Pod::ST_Flight_Accel() {
	std::cout << "Entering: Flight Accel" << std::endl;

	// TODO implement here
}

void Pod::ST_Flight_Coast() {
	std::cout << "Entering: Flight Coast" << std::endl;
	
	// TODO implement here
}

void Pod::ST_Flight_Brake() {
	std::cout << "Entering: Flight Brake" << std::endl;
	
	// TODO implement here
}
