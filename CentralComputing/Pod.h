#ifndef _POD_H
#define _POD_H

#include "StateMachineCompact/StateMachine.h"
#include <iostream>


class Pod : public StateMachine {
	public:	
		/** 
		* Constructs a pod state machine
		**/
		Pod() : StateMachine(ST_MAX_STATES) {}

		/**
		* User controlled movement events
		**/
		void move_functional_tests();
		void move_safe_mode();
		void move_flight_acceleration();
		void emergency_brake();

		/**
		* Software controlled events
		**/
		void coast();
		void brake();

	private:
	
		void ST_Safe_Mode(EventData*);
		void ST_Functional_Test(EventData*);
		void ST_Flight_Accel(EventData*);
		void ST_Flight_Coast(EventData*);
		void ST_Flight_Brake(EventData*);

		BEGIN_STATE_MAP
			STATE_MAP_ENTRY(&Pod::ST_Safe_Mode)
			STATE_MAP_ENTRY(&Pod::ST_Functional_Test)
			STATE_MAP_ENTRY(&Pod::ST_Flight_Accel)
			STATE_MAP_ENTRY(&Pod::ST_Flight_Coast)
			STATE_MAP_ENTRY(&Pod::ST_Flight_Brake)
		END_STATE_MAP


		enum E_States
		{
			ST_SAFE_MODE = 0,
			ST_FUNCTIONAL_TEST,
			ST_FLIGHT_ACCEL,
			ST_FLIGHT_COAST,
			ST_FLIGHT_BRAKE,
			ST_MAX_STATES
		};


};

#endif
