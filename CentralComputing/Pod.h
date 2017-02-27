#include <StateMachine.h>

class Pod : public StateMachine {
	public:	
		/** 
		* Constructs a pod state machine
		**/
		Pod();

		/**
		* User controlled movement events
		**/
		void move_functional_tests();
		void move_safe_mode()
		void move_flight_acceleration();
		void emergency_brake();

		/**
		* Software controlled events
		**/
		void coast();
		void brake();

	private:
		enum States
		{
			ST_SAFE_MODE,
			ST_FUNCTIONAL_TEST,
			ST_FLIGHT_ACCEL,
			ST_FLIGHT_COAST,
			ST_FLIGHT_BRAKE,
			ST_MAX_STATES
		}

		//void ST_Safe_Mode(const NoEventData*);
		//StateAction<Pod, NoEventData, &Pod::ST_Safe_Mode> Safe_Mode;
		STATE_DECLARE(Pod,	Safe_Mode,			NoEventData);
		STATE_DECLARE(Pod,	Functional_Test,	NoEventData);
		STATE_DECLARE(Pod,	Flight_Accel,		NoEventData);
		STATE_DECLARE(Pod,	Flight_Coast,		NoEventData);
		STATE_DECLARE(Pod,	Flight_Brake,		NoEventData);

		BEGIN_STATE_MAP
			STATE_MAP_ENTRY(&Safe_Mode)
			STATE_MAP_ENTRY(&Functional_Test)
			STATE_MAP_ENTRY(&Flight_Accel)
			STATE_MAP_ENTRY(&Flight_Coast)
			STATE_MAP_ENTRY(&Flight_Brake)
		END_STATE_MAP




};
