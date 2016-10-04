// Copyright 2010 Christophe Henry
// henry UNDERSCORE christophe AT hotmail DOT com
// This is an extended version of the state machine available in the boost::mpl library
// Distributed under the same license as the original.
// Copyright for the original version:
// Copyright 2005 David Abrahams and Aleksey Gurtovoy. Distributed
// under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

using namespace std;
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

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

        // the initial state of the player SM. Must be defined
        typedef SafeMode initial_state;

        // transition actions
        // as the functors are generic on events, fsm and source/target state, 
        // you can reuse them in another machine if you wish
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
            Row < FlightCoast , flight_brake, FlightBrake , none                  , flightC_flightB       >,
            //  +-------------+-------------+-------------+-----------------------+----------------------+
            Row < FlightBrake , user_select , SafeMode    , none                  , flightB_safe          >
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

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Safe", "init", "FunctA", "functB", "functC", "functD", "Loading", "Flight Accel" , "Flight Coast", "Flight Brake"};
    void pstate(pod const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void test()
    {        
        pod p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start(); 
        // go to Open, call on_exit on Empty, then action, then on_entry on Open
	std::cout << "user select safe mode"<<std::endl;
        p.process_event(
            user_select(SAFE_MODE)); pstate(p);
	std::cout << "user select init sensors"<<std::endl;
        p.process_event(
            user_select(INIT_SENSORS)); pstate(p);
	std::cout << "user select funct A"<<std::endl;
        p.process_event(
            user_select(FUNCT_A)); pstate(p);
	std::cout << "user select funct B"<<std::endl;
        p.process_event(
            user_select(FUNCT_B)); pstate(p);
	std::cout << "user select funct C"<<std::endl;
        p.process_event(
            user_select(FUNCT_C)); pstate(p);
	std::cout << "user select funct D"<<std::endl;
        p.process_event(
            user_select(FUNCT_D)); pstate(p);
	std::cout << "user select Loading"<<std::endl;
        p.process_event(
            user_select(LOADING)); pstate(p);
	std::cout << "user select funct A"<<std::endl;
        p.process_event(
            user_select(FUNCT_A)); pstate(p);
	std::cout << "user select funct B"<<std::endl;
        p.process_event(
            user_select(FUNCT_B)); pstate(p);
	std::cout << "user select funct C"<<std::endl;
        p.process_event(
            user_select(FUNCT_C)); pstate(p);
	std::cout << "user select funct D"<<std::endl;
        p.process_event(
            user_select(FUNCT_D)); pstate(p);
	std::cout << "user select flight A"<<std::endl;
        p.process_event(
            user_select(FLIGHT_A)); pstate(p);
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
        std::cout << "stop fsm" << std::endl;
        p.stop();

    }
}

int main()
{
    test();
    return 0;
}