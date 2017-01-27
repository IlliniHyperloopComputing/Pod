#ifndef FSM_EVENTS
#define FSM_EVENTS


// A "complicated" event type that carries some data.
enum IncomingCommandEnum
{
    SAFE_MODE=0,
    FUNCTIONAL_TEST=1,
	FLIGHT=2,
	BRAKING=3, 
	LEV_MOTOR=4,
	STA_MOTOR=5,
	ARM_LEV_MOTOR=6,
	ARM_STA_MOTOR=7,
	RESET_SENSORS=8,
    DISCONNECT=9

	
};

struct flight_brake {};
struct command
{
    command(IncomingCommandEnum c, int val)
        : command_type(c), command_value(val){} 
    command(IncomingCommandEnum c)
        : command_type(c), command_value(0){}

    IncomingCommandEnum command_type;
    int command_value;
};

#endif
