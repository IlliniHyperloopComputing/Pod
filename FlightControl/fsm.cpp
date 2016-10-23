//threading
#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/atomic.hpp>

//Network
#include "server.h"

//Sensor
#include "sensor.h"


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

// check this
#include "motors/motors.h"


using namespace std;
//MSM
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

// objects for motors go here


//Threading / queue
typedef boost::shared_ptr<user_select> user_select_ptr;
typedef boost::lockfree::spsc_queue<user_select_ptr, boost::lockfree::capacity<1024> > user_queue;
user_queue queue;

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
        struct TestFct 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&, FSM&,SourceState& ,TargetState& )
            {
                cout << "transition with event:" << typeid(EVT).name() << endl;
            }
        };
        struct start_playback 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::start_playback" << endl;
            }
        };
        struct store_cd_info 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "player::store_cd_info" << endl;
                //fsm.process_event(play());
            }
        };

 
        typedef pod_ p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start         Event         Next          Action                  Guard
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < SafeMode    , user_select , FunctionalA , none                  , safe_functA           >,
            Row < SafeMode    , user_select , InitSensors , none                  , safe_init_sensors     >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < InitSensors , user_select , FunctionalA , none                  , init_functA           >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalA , user_select , FunctionalB , none                  , functA_functB         >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalB , user_select , FunctionalC , none                  , functB_functC         >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalC , user_select , FunctionalD , none                  , functC_functD         >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FunctionalD , user_select , SafeMode    , none                  , functD_safe           >,
            Row < FunctionalD , user_select , Loading     , none                  , functD_loading        >,
            Row < FunctionalD , user_select , FlightAccel , none                  , functD_flightA        >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < Loading     , user_select , FunctionalA , none                  , loading_functA        >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FlightAccel , user_select , SafeMode    , none                  , flightA_safe          >,
            Row < FlightAccel , flight_coast, FlightCoast , none                  , flightA_flightC       >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FlightCoast , flight_brake, FlightBrake , none                  , flightC_flightB       >
            //  +-------------+-------------+-------------+-----------------------+----------------------+
 //           Row < FlightBrake , user_select , SafeMode    , none                  , flightB_safe          >,
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
// // Testing utilities.  //
    static char const* const state_names[] = { "Safe", "init", "FunctA", "functB", "functC", "functD", "Loading", "Flight Accel" , "Flight Coast", "Flight Brake"};
    void pstate(pod const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void state_machine_loop(void)
    {        
        cout <<"Running FSM"<<endl;
        pod p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start(); 
        // go to Open, call on_exit on Empty, then action, then on_entry on Open
//	std::cout << "user select safe mode"<<std::endl;
//        p.process_event(
//            user_select(SAFE_MODE)); pstate(p);
//	std::cout << "user select init sensors"<<std::endl;
//        p.process_event(
//            user_select(INIT_SENSORS)); pstate(p);
//	std::cout << "user select funct A"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_A)); pstate(p);
//	std::cout << "user select funct B"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_B)); pstate(p);
//	std::cout << "user select funct C"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_C)); pstate(p);
//	std::cout << "user select funct D"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_D)); pstate(p);
//	std::cout << "user select Loading"<<std::endl;
//        p.process_event(
//            user_select(LOADING)); pstate(p);
//	std::cout << "user select funct A"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_A)); pstate(p);
//	std::cout << "user select funct B"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_B)); pstate(p);
//	std::cout << "user select funct C"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_C)); pstate(p);
//	std::cout << "user select funct D"<<std::endl;
//        p.process_event(
//            user_select(FUNCT_D)); pstate(p);
//	std::cout << "user select flight A"<<std::endl;
//        p.process_event(
//            user_select(FLIGHT_A)); pstate(p);
        /*p.process_event(open_close()); pstate(p);
        // will be rejected, wrong disk type
        p.process_event(
            cd_detected("louie, louie",DISK_DVD)); pstate(p);
        p.process_event(
            cd_detected("louie, louie",DISK_CD)); pstate(p);
        // no need to call play() as the previous event does it in its action method
        //p.process_event(play());

        // at this point, Play is active      
        p.process_event(pause()); pstate(p);
        // go back to Playing
        p.process_event(end_pause());  pstate(p);
        p.process_event(pause()); pstate(p);
        p.process_event(stop());  pstate(p);
        // event leading to the same state
        // no action method called as it is not present in the transition table
        */
        //p.process_event(stop());  pstate(p);

        while(1){
            user_select_ptr usp;
            queue.pop(usp);
            if(usp){
                p.process_event(*usp); pstate(p);
            }
        }
        std::cout << "stop fsm" << std::endl;
        p.stop();

    }
}

void sensor_loop(void){
    cout <<"Running Sensor loop"<<endl; 
    //infinate loop is not good idea. come up with something else
    while(1){
        sen->update();
    }
}

void network_connect(void){
    server = new tcp_server(ioservice, &queue, sen);
    cout <<"Running network service"<<endl;
    ioservice.run();
}

int main()
{
    cout << "boost::lockfree::queue is ";
    if (!queue.is_lock_free())
        cout << "not ";
    cout << "lockfree" << endl;

    sen = new sensor();
    boost::thread sensor_thread(sensor_loop);
    boost::thread state_machine_thread(state_machine_loop);
    boost::thread network_thread(network_connect);


    state_machine_thread.join();
    network_thread.join();
    sensor_thread.join();
    delete server;
    delete sen;
    return 0;
}
