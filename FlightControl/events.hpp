#ifndef FSM_EVENTS
#define FSM_EVENTS

struct flight_coast {};
struct flight_brake {};
struct init_sensors {};//used to start sensor orthagonal machine

// A "complicated" event type that carries some data.
enum StateSelectEnum
{
    SAFE_MODE=0,
    INIT_SENSORS=1,
    FUNCT_A=2,
    FUNCT_B=3,
    FUNCT_C=4,
    FUNCT_D=5,
    LOADING=6,
    FLIGHT_A=7,
    E_BRAKE=8
};
struct user_select
{
    user_select(StateSelectEnum state)
        : state_select(state)
    {}
        
    StateSelectEnum state_select;

};


//submachine
struct state_swap
{

};

#endif
