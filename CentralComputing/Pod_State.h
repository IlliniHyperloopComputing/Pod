#ifndef _POD_STATE
#define _POD_STATE

#include "StateMachine.h"
#include <iostream>
#include <string>
#include <SafeQueue.hpp>

class Pod_State : public StateMachine {
	public:

		enum E_States
		{
			ST_POD_STARTUP,
			ST_SAFETY_SETUP,
			ST_SAFE_MODE,
			ST_FUNCTIONAL_TEST,
			ST_LAUNCH_READY,
			ST_FLIGHT_ACCEL,
			ST_FLIGHT_COAST,
			ST_FLIGHT_BRAKE,
			ST_MAX_STATES
		};

		/** 
		* Constructs a pod state machine
		**/
		Pod_State(SafeQueue<Xmega_Command_t> * cmd_queue) 
      : StateMachine(ST_MAX_STATES), 
        command_queue(cmd_queue)
    {}

		// returns the current state as an enum
		E_States get_current_state();
		
		// returns the string name for current state
		std::string get_current_state_string() {
			std::string states[] =
			{
				"POD_STARTUP",
				"SAFETY_SETUP",
				"SAFE_MODE",
				"FUNCTIONAL_TESTS",
				"LAUNCH_READY",
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
		void move_safety_setup();
		void move_functional_tests();
		void move_safe_mode();
		void move_launch_ready();
		void emergency_brake();

		/**
		* Software controlled events
		**/
		void accelerate();
		void coast();
		void brake();
		
	private:
		void ST_Pod_Startup();
		void ST_Safety_Setup();
		void ST_Safe_Mode();
		void ST_Functional_Test();
		void ST_Launch_Ready();
		void ST_Flight_Accel();
		void ST_Flight_Coast();
		void ST_Flight_Brake();

		BEGIN_STATE_MAP
			STATE_MAP_ENTRY(&Pod_State::ST_Pod_Startup)
			STATE_MAP_ENTRY(&Pod_State::ST_Safety_Setup)
			STATE_MAP_ENTRY(&Pod_State::ST_Safe_Mode)
			STATE_MAP_ENTRY(&Pod_State::ST_Functional_Test)
			STATE_MAP_ENTRY(&Pod_State::ST_Launch_Ready)
			STATE_MAP_ENTRY(&Pod_State::ST_Flight_Accel)
			STATE_MAP_ENTRY(&Pod_State::ST_Flight_Coast)
			STATE_MAP_ENTRY(&Pod_State::ST_Flight_Brake)
		END_STATE_MAP

    SafeQueue<Xmega_Command_t> * command_queue;

};

#endif
