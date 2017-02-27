#include "Pod.h"

Pod::Pod() :
	StateMachine(ST_MAX_STATES)
{
	//Any other constructor work
}

void Pod::move_functional_tests(){
	BEGIN_TRANSITION_MAP							/* Current state */
		TRANSITION_MAP_ENTRY(ST_FUNCTIONAL_TEST)	/* Safe mode */
		TRANSITION_MAP_ENTRY(ST_FUNCTIONAL_TEST)	/* Functional test */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight accel */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight coast */
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)			/* Flight brake */
	END_TRANSITION_MAP(NULL)
}
