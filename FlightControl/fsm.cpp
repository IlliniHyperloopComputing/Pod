//threading
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/atomic.hpp>

//Network
#include "server.h"

//Sensor
#include "sensor.h"

//Active controls
#include "active.h"

//BlackLib
#include "../BlackLib/v3_0/BlackPWM/BlackPWM.h"
#include "../BlackLib/v3_0/BlackGPIO/BlackGPIO.h"

#include <vector>
#include <iostream>
// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>

#include "guards.hpp"
#include "events.hpp"
#include "server.h"

#include "motors.h"
#include "brakes.h"

#include "status.h"

using namespace std;
//MSM
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

//Object for active controls (motors and brakes)
active * act;

//Threading / queue
typedef boost::shared_ptr<command> command_ptr;
typedef boost::lockfree::spsc_queue<command_ptr, boost::lockfree::capacity<1024> > command_queue;
command_queue incoming_command_queue;

status_queue fsm_status_queue;

tcp_server * server;
boost::asio::io_service ioservice;
sensor * sen;

namespace  // Concrete FSM implementation
{
    // front-end: define the FSM structure 
    struct pod_ : public msm::front::state_machine_def<pod_>
    {
        template <class Event,class FSM>
        void on_entry(Event const& ,FSM&) 
        {
            std::cout << "entering: Pod" << std::endl;
        }
        template <class Event,class FSM>
        void on_exit(Event const&,FSM& ) 
        {
            std::cout << "leaving: Pod" << std::endl;
        }

        // The list of FSM states
        struct SafeMode : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: SafeMode" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: SafeMode" << std::endl;
            }
        };
        struct InitSensors : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: InitSensors" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: InitSensors" << std::endl;
            }
        };

        struct EBrake : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: EBrake" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: EBrake" << std::endl;
            }
        };

        struct FunctionalA : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FunctionalA" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FunctionalA" << std::endl;
            }
        };

        struct FunctionalB : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FunctionalB" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FunctionalB" << std::endl;
            }
        };

        struct FunctionalC : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FunctionalC" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FunctionalC" << std::endl;
            }
        };

        struct FunctionalD : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FunctionalD" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FunctionalD" << std::endl;
            }
        };

        struct Loading : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: Loading" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: Loading" << std::endl;
            }
        };

        struct FlightAccel : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FlightAccel" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FlightAccel" << std::endl;
            }
        };
        struct FlightCoast : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FlightCoast" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FlightCoast" << std::endl;
            }
        };
        struct FlightBrake : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FlightBrake" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FlightBrake" << std::endl;
            }
        };

 //       struct SensorsWait: public msm::front::state<>
 //       {
 //           template <class Event,class FSM>
 //           void on_entry(Event const&,FSM& )
 //           {
 //               std::cout << "entering: SensorsWait" << std::endl;
 //           }

 //           template <class Event,class FSM>
 //           void on_exit(Event const&,FSM& )
 //           {
 //               std::cout << "leaving: SensorsWait" << std::endl;
 //           }
 //       };
 //       struct SensorsInit: public msm::front::state<>
 //       {
 //           template <class Event,class FSM>
 //           void on_entry(Event const&,FSM& )
 //           {
 //               std::cout << "entering: SensorsInit" << std::endl;
 //           }

 //           template <class Event,class FSM>
 //           void on_exit(Event const&,FSM& )
 //           {
 //               std::cout << "leaving: SensorsInit" << std::endl;
 //           }
 //       };
 //       struct SensorsGetData: public msm::front::state<>
 //       {
 //           template <class Event,class FSM>
 //           void on_entry(Event const&,FSM& )
 //           {
 //               std::cout << "entering: SensorsGetData" << std::endl;
 //           }

 //           template <class Event,class FSM>
 //           void on_exit(Event const&,FSM& )
 //           {
 //               std::cout << "leaving: SensorsGetData" << std::endl;
 //           }
        //};

    

        // the initial state of the player SM. Must be defined
        typedef SafeMode initial_state;
        //typedef mpl::vector<SafeMode,SensorsWait> initial_state;

        // transition actions
        // as the functors are generic on events, fsm and source/target state, 
        // you can reuse them in another machine if you wish
 //       struct turn_on_sensors 
 //       {
 //           template <class EVT,class FSM,class SourceState,class TargetState>
 //           void operator()(EVT const&, FSM&,SourceState& ,TargetState& )
 //           {
 //               cout << "turn_on_sensors-- transition with event:" << typeid(EVT).name() << endl;
 //           }
 //       };


	// performs neccesary actions when moving into loading 
        struct to_loading 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into loading state" << endl;
		//motor_levitation->disarm();
		//motor_stability->disarm();
            }
        };

	// performs actions when moving into flight_acceleration
        struct to_flight_accel
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into flight accel state" << endl;
         //       motor_levitation->arm();
		//motor_stability->arm();
            }
        };

	
	// performs actions when moving into flight_coast
        struct to_flight_coast
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into flight coast state" << endl;
         //       motor_levitation->arm();
		// needs to implement levitation profile
		//motor_stability->arm();
            }
        };


	// performs actions when moving into flight_brake
        struct to_flight_brake
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into flight brake state" << endl;
         //       motor_levitation->disarm();
		//        motor_stability->arm();
            }
        };


	// performs actions when moving into FunctionalA
        struct to_FunctionalA
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into FunctionalA state" << endl;
            }
        };


	// performs actions when moving into FunctionalB
        struct to_FunctionalB
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into FunctionalB state" << endl;
            }
        };

	// performs actions when moving into FunctionalC
        struct to_FunctionalC
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into FunctionalC state" << endl;
            }
        };

	// performs actions when moving into FunctionalD
        struct to_FunctionalD
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into FunctionalD state" << endl;
            }
        };

	// performs actions when moving into SafeMode
        struct to_SafeMode
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into SafeMode state" << endl;
            }
        };

	// performs actions when moving into InitSensors
        struct to_InitSensors
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "moving into InitSensors state" << endl;
            }
        };

        typedef pod_ p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start         Event         Next          Action                  Guard
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < SafeMode    , command , FunctionalA , to_FunctionalA        , safe_functA           >,
            Row < SafeMode    , command , InitSensors , to_InitSensors        , safe_init_sensors     >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < InitSensors , command , FunctionalA , to_InitSensors        , init_functA           >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalA , command , FunctionalB , to_FunctionalB        , functA_functB         >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalB , command , FunctionalC , to_FunctionalC        , functB_functC         >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalC , command , FunctionalD , to_FunctionalD        , functC_functD         >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalD , command , SafeMode    , to_SafeMode           , functD_safe           >,
            Row < FunctionalD , command , Loading     , to_loading            , functD_loading        >,
            Row < FunctionalD , command , FlightAccel , to_flight_accel       , functD_flightA        >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < Loading     , command , FunctionalA , to_FunctionalA        , loading_functA        >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FlightAccel , command , SafeMode    , to_SafeMode           , flightA_safe          >,
            Row < FlightAccel , flight_coast, FlightCoast , to_flight_coast   , flightA_flightC       >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FlightCoast , flight_brake, FlightBrake , to_flight_brake   , flightC_flightB       >
            //  +-------------+-------------+-------------+-----------------------+----------------------+
 //           Row < FlightBrake , command , SafeMode  , to_SafeMode           , flightB_safe          >,
 //           //  +-------------+-------------+-------------+-----------------------+----------------------+
 //           Row < SensorsWait , init_sensors, SensorsInit , turn_on_sensors       , none                  >
            /*

            Row < Stopped     , play        , Playing , ActionSequence_
                                                     <mpl::vector<
                                                     TestFct,start_playback> >            
                                                                              , DummyGuard           >,
            Row < Empty   , cd_detected , Stopped , store_cd_info             , And_<good_disk_format,
                                                                                     always_true>    >,
            // we here also mix with some "classical row"
          g_row < Empty   , cd_detected , Playing                             , &p::auto_start       >,
            */
        > {};
	//let's define a submachine!
	struct FlightCoast_ : public msm::front::state_machine_def<FlightCoast_>{
        struct state1 : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: state1" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: state1" << std::endl;
            }
        };
        struct state2 : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: state2" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: state2" << std::endl;
            }
        };
		typedef state1 initial_state; 	
		struct transition_table : mpl::vector<

            //    Start         Event         Next          Action                 
            //  +-------------+-------------+-------------+-----------------------+
            Row < state1      , state_swap        , state2      , none                  >,
	        Row < state2      , state_swap        , state1      , none                  >
	    >{};
		
        };
    typedef msm::back::state_machine<FlightCoast_> Coasting;
        // Replaces the default no-transition response.
        template <class FSM,class Event>
        void no_transition(Event const& e, FSM&,int state)
        {
            std::cout << "no transition from state " << state
                << " on event " << typeid(e).name() << std::endl;
        }
    };
    // Pick a back-end
    typedef msm::back::state_machine<pod_> pod;

    static char const* const state_names[] = { "Safe", "init", "FunctA", "functB", "functC", "functD", "Loading", "Flight Accel" , "Flight Coast", "Flight Brake"};
    void pstate(pod const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void state_machine_loop(void)
    {        
        printf("Running FSM\n");
        pod p;
        p.start(); 

        while(1){
            command_ptr cp;
            incoming_command_queue.pop(cp);
            if(cp){
                printf("Received command %d with value %d\n", cp->command_type, cp->command_value);

                if(cp->command_type == LEV_MOTOR){
                    act->set_lev(cp->command_value);
                    //motor_levitation->set_microseconds(cp->command_value); 
                } else if(cp->command_type == STA_MOTOR) {
                    act->set_sta(cp->command_value);
                    //motor_stability->set_microseconds(cp->command_value);
                } else if(cp->command_type == ARM_LEV_MOTOR) {
                    if(cp->command_value ==0){
                        act->off_lev();
                        //motor_levitation->off();
                    }
                    else{
                        act->on_lev();
                        //motor_levitation->on();
                    }
                } else if(cp->command_type == OFF) {
                    act->low_lev();
                    act->low_sta();
                    //motor_levitation->set_low();
                    //motor_stability->set_low();
                } else if(cp->command_type == BRAKE) {
                    int val = cp->command_value;
                    if(val==0)
                        act->stop_brake();
                    if(val==1)
                        act->forward_brake();
                    if(val==2)
                        act->backward_brake();
                }  else { 
                    p.process_event(*cp); 
                    pstate(p);
                }
            
            }
        }

        printf("Stoping FSM\n");
        p.stop();
    }

}


void sensor_loop(void){
    printf("Running Sensor Loop\n");
    //infinate loop is not good idea. come up with something else
    while(1){
        //usleep(30000);
        sen->update();
    }
}

void network_connect(void){
    printf("Setting up Network\n");
    server = new tcp_server(ioservice, &incoming_command_queue, &fsm_status_queue, sen);
    printf("Running Network\n");
    ioservice.run();
}

int main()
{
    sen = new sensor();
    act = new active();

    boost::thread sensor_thread(sensor_loop);
    boost::thread state_machine_thread(state_machine_loop);
    boost::thread network_thread(network_connect);

    state_machine_thread.join();
    network_thread.join();
    sensor_thread.join();

    delete server;
    delete act;
    delete sen;

    return 0;
}
