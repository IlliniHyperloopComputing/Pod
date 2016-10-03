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

using namespace std;
namespace msm = boost::msm;
namespace mpl = boost::mpl;
using namespace msm::front;
// for And_ operator
using namespace msm::front::euml;

namespace  // Concrete FSM implementation
{
    /*struct play {};
    struct end_pause {};
    struct stop {};
    struct pause {};
    struct open_close {};
    */
    struct flight_coast {};
    struct flight_brake {};

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
                std::cout << "entering: SafeMode" << std::endl;
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
                std::cout << "entering: InitSensors" << std::endl;
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
                std::cout << "entering: EBrake" << std::endl;
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
                std::cout << "entering: FunctionalA" << std::endl;
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
                std::cout << "entering: FunctionalB" << std::endl;
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
                std::cout << "entering: FunctionalC" << std::endl;
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
                std::cout << "entering: FunctionalD" << std::endl;
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
                std::cout << "entering: Loading" << std::endl;
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
                std::cout << "entering: FlightAccel" << std::endl;
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
                std::cout << "entering: FlightCoast" << std::endl;
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
                std::cout << "entering: FlightBrake" << std::endl;
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
        struct open_drawer 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::open_drawer" << endl;
            }
        };
        struct close_drawer 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::close_drawer" << endl;
            }
        };
        struct store_cd_info 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const&,FSM& fsm ,SourceState& ,TargetState& )
            {
                cout << "player::store_cd_info" << endl;
                fsm.process_event(play());
            }
        };
        struct stop_playback 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::stop_playback" << endl;
            }
        };
        struct pause_playback 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::pause_playback" << endl;
            }
        };
        struct resume_playback 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::resume_playback" << endl;
            }
        };
        struct stop_and_open 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::stop_and_open" << endl;
            }
        };
        struct stopped_again 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            void operator()(EVT const& ,FSM& ,SourceState& ,TargetState& )
            {
                cout << "player::stopped_again" << endl;
            }
        };
        // guard conditions
        struct DummyGuard 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            bool operator()(EVT const& evt,FSM& fsm,SourceState& src,TargetState& tgt)
            {
                return true;
            }
        };
        struct good_disk_format 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
            {
                // to test a guard condition, let's say we understand only CDs, not DVD
                if (evt.disc_type != DISK_CD)
                {
                    std::cout << "wrong disk, sorry" << std::endl;
                    return false;
                }
                return true;
            }
        };
        struct always_true 
        {
            template <class EVT,class FSM,class SourceState,class TargetState>
            bool operator()(EVT const& evt ,FSM&,SourceState& ,TargetState& )
            {             
                return true;
            }
        };
        // we want to define one row with the classic look.
        bool auto_start(cd_detected const& evt)
        {
            return false;
        }
 
        typedef pod_ p; // makes transition table cleaner

        // Transition table for player
        struct transition_table : mpl::vector<
            //    Start     Event         Next          Action                     Guard
            //  +---------+-------------+-------------+-----------------------+----------------------+
            Row < SafeMode, user_select , InitSensors , none                  , none                 >,
            Row < SafeMode, user_select , InitSensors , none                  , none                 >,
            Row < Stopped , play        , Playing , ActionSequence_
                                                     <mpl::vector<
                                                     TestFct,start_playback> >            
                                                                              , DummyGuard           >,
            Row < Stopped , open_close  , Open    , open_drawer               , none                 >,
            Row < Stopped , stop        , Stopped , none                      , none                 >,
            //  +---------+-------------+---------+---------------------------+----------------------+
            Row < Open    , open_close  , Empty   , close_drawer              , none                 >,
            //  +---------+-------------+---------+---------------------------+----------------------+
            Row < Empty   , open_close  , Open    , open_drawer               , none                 >,
            Row < Empty   , cd_detected , Stopped , store_cd_info             , And_<good_disk_format,
                                                                                     always_true>    >,
            // we here also mix with some "classical row"
          g_row < Empty   , cd_detected , Playing                             , &p::auto_start       >,
            //  +---------+-------------+---------+---------------------------+----------------------+
            Row < Playing , stop        , Stopped , stop_playback             , none                 >,
            Row < Playing , pause       , Paused  , pause_playback            , none                 >,
            Row < Playing , open_close  , Open    , stop_and_open             , none                 >,
            //  +---------+-------------+---------+---------------------------+----------------------+
            Row < Paused  , end_pause   , Playing , resume_playback           , none                 >,
            Row < Paused  , stop        , Stopped , stop_playback             , none                 >,
            Row < Paused  , open_close  , Open    , stop_and_open             , none                 >
            //  +---------+-------------+---------+---------------------------+----------------------+
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
    typedef msm::back::state_machine<pod_> player;

    //
    // Testing utilities.
    //
    static char const* const state_names[] = { "Stopped", "Open", "Empty", "Playing", "Paused" };
    void pstate(player const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }

    void test()
    {        
        player p;
        // needed to start the highest-level SM. This will call on_entry and mark the start of the SM
        p.start(); 
        // go to Open, call on_exit on Empty, then action, then on_entry on Open
        p.process_event(open_close()); pstate(p);
        p.process_event(open_close()); pstate(p);
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
        p.process_event(stop());  pstate(p);
        std::cout << "stop fsm" << std::endl;
        p.stop();

    }
}

int main()
{
    test();
    return 0;
}
