#ifndef _POD_STATE
#define _POD_STATE

#include "StateMachine.h"
#include <iostream>
#include <string>
#include <SafeQueue.hpp>
#include "Sensor.h"

class Pod_State : public StateMachine {
	public:

		enum E_States
		{
			ST_SAFE_MODE,
			ST_FUNCTIONAL_TEST,
      ST_LOADING,
			ST_LAUNCH_READY,
			ST_FLIGHT_ACCEL,
			ST_FLIGHT_COAST,
			ST_FLIGHT_BRAKE,
			ST_MAX_STATES
		};

		/** 
		* Constructs a pod state machine
		**/
		Pod_State() 
      : StateMachine(ST_MAX_STATES)
    {}

		// returns the current state as an enum
		E_States get_current_state();
		
		// returns the string name for current state
		std::string get_current_state_string() {
			std::string states[] =
			{
				"SAFE_MODE",
				"FUNCTIONAL_TESTS",
        "LOADING",
				"LAUNCH_READY",
				"FLIGHT_ACCEL",
				"FLIGHT_COAST",
				"FLIGHT_BRAKE",
				"NOT A STATE"
			};
			return states[(int)getCurrentState()];
		}
		
		/**
		* User controlled movement events
		**/
		void move_safe_mode();
		void move_functional_tests();
    void move_loading();
		void move_launch_ready();
		void accelerate();
		void emergency_brake();

		/**
		* Software controlled events
		**/
		void coast();
		void brake();
		
	private:
		void ST_Safe_Mode();
		void ST_Functional_Test();
    void ST_Loading();
		void ST_Launch_Ready();
		void ST_Flight_Accel();
		void ST_Flight_Coast();
		void ST_Flight_Brake();

		BEGIN_STATE_MAP
			STATE_MAP_ENTRY(&Pod_State::ST_Safe_Mode)
			STATE_MAP_ENTRY(&Pod_State::ST_Functional_Test)
			STATE_MAP_ENTRY(&Pod_State::ST_Loading)
			STATE_MAP_ENTRY(&Pod_State::ST_Launch_Ready)
			STATE_MAP_ENTRY(&Pod_State::ST_Flight_Accel)
			STATE_MAP_ENTRY(&Pod_State::ST_Flight_Coast)
			STATE_MAP_ENTRY(&Pod_State::ST_Flight_Brake)
		END_STATE_MAP

};

#endif
