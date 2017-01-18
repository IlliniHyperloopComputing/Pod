#ifndef FSM_EVENTS
#define FSM_EVENTS

struct event{};
struct flight_coast {};
struct flight_brake {};
struct init_sensors {};//used to start sensor orthagonal machine

// A "complicated" event type that carries some data.
enum IncomingCommandEnum
{
    SAFE_MODE=0,
    INIT_SENSORS=1,
    FUNCT_A=2,
    FUNCT_B=3,
    FUNCT_C=4,
    FUNCT_D=5,
    LOADING=6,
    FLIGHT_A=7,
    E_BRAKE=8,
    LEV_MOTOR=9,
    STA_MOTOR = 10,
    ARM_LEV_MOTOR = 11,
    ARM_STA_MOTOR = 12,
    OFF = 13,
    BRAKE = 14
};
/*
struct user_select
{
    user_sel`ect(StateSelectEnum state)
        : state_select(state)
    {}
        
    StateSelectEnum state_select;

};*/

struct command
{
    command(IncomingCommandEnum c, int val)
        : command_type(c), command_value(val){} 
    command(IncomingCommandEnum c)
        : command_type(c), command_value(0){}

    IncomingCommandEnum command_type;
    int command_value;
};


//submachine
struct state_swap
{

};

#endif
