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
//Object for the sensors
sensor * sen;

//Threading / queue
typedef boost::shared_ptr<command> command_ptr;
typedef boost::lockfree::spsc_queue<command_ptr, boost::lockfree::capacity<1024> > command_queue;
command_queue incoming_command_queue;

//Typedefs for this queue are found within status.h
status_queue fsm_status_queue;

tcp_server * server;
boost::asio::io_service ioservice;

std::vector<status_message_ptr> tmp_status_buff;
boost::mutex sensor_mutex;


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

        struct FunctionalTest : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: FunctionalTest" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: FunctionalTest" << std::endl;
            }
        };

        struct Flight : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: Flight" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: Flight" << std::endl;
            }
        };

        struct Braking : public msm::front::state<>
        {
            template <class Event,class FSM>
            void on_entry(Event const&,FSM& )
            {
                std::cout << "entering: Braking" << std::endl;
            }

            template <class Event,class FSM>
            void on_exit(Event const&,FSM& )
            {
                std::cout << "leaving: Braking" << std::endl;
            }
        };
	
    	// the initial state of the player SM. Must be defined
    	typedef SafeMode initial_state;
		// performs neccesary actions when moving into loading 	
		struct to_functional {	
			template <class EVT, class FSM, class SourceState, class TargetState>
			void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
				cout << "moving into functionaltests" << endl;
			}
		};
		struct to_flight {
			template <class EVT, class FSM, class SourceState, class TargetState>
			void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
				cout << "moving into flight" << endl;
			}
		};
		struct to_braking {	
			template <class EVT, class FSM, class SourceState, class TargetState>
			void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
				cout << "moving into braking" << endl;
			}
		};
		struct to_safe_mode {	
			template <class EVT, class FSM, class SourceState, class TargetState>
			void operator()(EVT const&, FSM& fsm, SourceState&, TargetState&) {
				cout << "moving into safe_mode" << endl;
			}
		};


		typedef pod_ p; // makes transition table cleaner

			// Transition table for player
		struct transition_table : mpl::vector<
				//      Start         Event         Next          Action           
				//    +-----------+-------------+---------------+------------------+
			Row < SafeMode      , command     , FunctionalTest, to_functional   >,
			Row < FunctionalTest, command     , Flight        , to_flight       >,
			Row < Flight	    , flight_brake, Braking       , to_braking      >,
			Row < Flight        , command     , Braking       , to_braking      >,
			Row < Braking       , command     , SafeMode      , to_safe_mode    >
			
			> {};
	};

	//let's define a submachine! //let's not
    typedef msm::back::state_machine<pod_> pod;
    static char const* const state_names[] = { "SafeMode", "FunctionalTest", "Flight", "Braking"};

    void pstate(pod const& p)
    {
        std::cout << " -> " << state_names[p.current_state()[0]] << std::endl;
    }
	void reset_sensors(){
		sensor_mutex.lock();
		sen->reset_tape_count();
		//TODO reset acceleration
		sensor_mutex.unlock();
	}

    void state_machine_loop(void)
    {        
        while (!tmp_status_buff.empty()) {
            fsm_status_queue.push(tmp_status_buff.back());
            tmp_status_buff.pop_back();
        }        

        printf("Running FSM\n");
        pod p;
        p.start(); 

        while(1){
            command_ptr cp;
            incoming_command_queue.pop(cp);
            if(cp){
                printf("Received command %d with value %d\n", cp->command_type, cp->command_value);

                status_message_ptr smp;
                if(cp->command_type == LEV_MOTOR){
                    //Set the motor
                    act->set_lev(cp->command_value);
                    //Send confermation of action
                    char tmp[5];
                    sprintf(tmp,"%4d",cp->command_value);
                    std::string l = "L";
                    smp = status_message_ptr(new status_message(STATUS_CONTROL,+tmp));

                    //motor_levitation->set_microseconds(cp->command_value); 
                } else if(cp->command_type == STA_MOTOR) {
                    act->set_sta(cp->command_value);

                    char tmp[5];
                    sprintf(tmp,"%4d",cp->command_value);
                    std::string s = "S";
                    smp = status_message_ptr(new status_message(STATUS_CONTROL,s+tmp));

                    //motor_stability->set_microseconds(cp->command_value);
                } else if(cp->command_type == ARM_LEV_MOTOR) {
                    if(cp->command_value ==0){
                        act->off_lev();

                        smp = status_message_ptr(new status_message(STATUS_CONTROL,"L0"));
                        //motor_levitation->off();
                    }
                    else{
                        act->on_lev();
                        smp = status_message_ptr(new status_message(STATUS_CONTROL,"L1"));
                        //motor_levitation->on();
                    }
                } else if(cp->command_type == ARM_STA_MOTOR) {
                    if(cp->command_value ==0){
                        act->off_sta();

                        smp = status_message_ptr(new status_message(STATUS_CONTROL,"S0"));
                        //motor_levitation->off();
                    }
                    else{
                        act->on_sta();
                        smp = status_message_ptr(new status_message(STATUS_CONTROL,"S1"));
                        //motor_levitation->on();
                    }	
				} else if(cp->command_type == RESET_SENSORS) {
					reset_sensors();	
                } else if(cp->command_type == SAFE_MODE) {
                    act->low_lev();
                    act->low_sta();
                    act->off_sta();
                    act->off_lev();

                    smp = status_message_ptr(new status_message(STATUS_CONTROL,"S0"));
                    smp = status_message_ptr(new status_message(STATUS_CONTROL,"L0"));
                } else if(cp->command_type == BRAKING) {
                    int val = cp->command_value;
                    if(val==0)
                        act->stop_brake();
                    if(val==1)
                        act->forward_brake();
                    if(val==2)
                        act->backward_brake();
                    char tmp[2];
                    sprintf(tmp,"%1d",val);
                    std::string b = "B";
                    smp = status_message_ptr(new status_message(STATUS_CONTROL,b+tmp));
                }  else { 
                    p.process_event(*cp); 
                    pstate(p);
                }
            fsm_status_queue.push(smp);
            
            }
        }

        printf("Stopping FSM\n");
        p.stop();
    }

	
}


void sensor_loop(void){
    printf("Running Sensor Loop\n");
    //infinate loop is not good idea. come up with something else
    while(1){
        //usleep(30000);
	sensor_mutex.lock();	
        sen->update();
	sensor_mutex.unlock();
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
    //Initialize the sensors
    sen = new sensor(&tmp_status_buff);
    //Initialize the active controls 
    act = new active();

    //Spin off threads
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
