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
/*
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors

#include <boost/msm/front/functor_row.hpp>
#include <boost/msm/front/euml/common.hpp>
// for And_ operator
#include <boost/msm/front/euml/operator.hpp>
*/

#include "guards.hpp"
#include "events.hpp"
#include "server.h"

#include "motors.h"
#include "brakes.h"

#include "status.h"

#include <sys/time.h>

using namespace std;
//MSM

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


enum State
{
	SAFE_MODE_STATE=0,
	FUNCTIONAL_TEST_STATE=1,
	FLIGHT_STATE=2,
	BRAKING_STATE=3
};

string state_names[] = { "SafeMode", "FunctionalTest", "Flight", "Braking"};
State state = SAFE_MODE_STATE;

bool should_brake() {
	// TODO Adjust 
	return state == FLIGHT_STATE && (*sen->get_distance() > 1600 || (*sen->get_distance() > 500 && abs(sen->get_atomic_a()[0]) < 0.2));
}

void brake(int val) {
	// TODO Deal with asymptote
	cout << "Braking" << endl;
	if(val==0)
		act->stop_brake();
	if(val==1 || val == 1111){

		double brake_pressure = *sen->get_brake_pressure();
		int counter = 0;
		int total_time = 0;
		const int A = 500000;
		while(brake_pressure <= 315 && total_time <= A * 1.99){
			act->forward_brake();
			//1 second total
			usleep(A/++counter);
			total_time += A/counter;
			act->stop_brake();
            brake_pressure = *sen->get_brake_pressure();
		}
	}
	if(val==2)
		act->backward_brake();
	state = BRAKING_STATE; 
	status_message_ptr smp;
	smp = status_message_ptr(new status_message(STATUS_STATE, state_names[state]));
    fsm_status_queue.push(smp);
}

void reset_sensors(){
	sensor_mutex.lock();
	sen->reset_sensors();
	sensor_mutex.unlock();
}

void state_machine_loop(void)
{        
	while (!tmp_status_buff.empty()) {
		fsm_status_queue.push(tmp_status_buff.back());
		tmp_status_buff.pop_back();
	}        

	printf("Running FSM\n");
	while(1){
		if(should_brake()){
			brake(1);
		}
		command_ptr cp;
		incoming_command_queue.pop(cp);
		if(cp){
			printf("Received command %d with value %d\n", cp->command_type, cp->command_value);

			status_message_ptr smp;
            
			if(cp->command_type == LEV_MOTOR && (state == FUNCTIONAL_TEST_STATE || state == FLIGHT_STATE)){
				//Set the motor
				act->set_lev(cp->command_value);
				//Send confermation of action
				char tmp[5];
				sprintf(tmp,"%4d",cp->command_value);
				std::string l = "L";
				smp = status_message_ptr(new status_message(STATUS_CONTROL,+tmp));

				//motor_levitation->set_microseconds(cp->command_value); 
			} else if(cp->command_type == STA_MOTOR && (state == FUNCTIONAL_TEST_STATE ||  state == FLIGHT_STATE)) {
				act->set_sta(cp->command_value);

				char tmp[5];
				sprintf(tmp,"%4d",cp->command_value);
				std::string s = "S";
				smp = status_message_ptr(new status_message(STATUS_CONTROL,s+tmp));

				//motor_stability->set_microseconds(cp->command_value);
			} else if(cp->command_type == ARM_LEV_MOTOR && (state == FUNCTIONAL_TEST_STATE || state == FLIGHT_STATE)) {
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
			} else if(cp->command_type == ARM_STA_MOTOR && (state == FUNCTIONAL_TEST_STATE ||  state == FLIGHT_STATE)) {
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
				if(state == FLIGHT_STATE || state == SAFE_MODE_STATE) {
                    string message = "Cannot return to safe mode"; 
					cout << message << endl; 
                    smp = status_message_ptr(new status_message(STATUS_ERROR, message));
				} else {
					cout << "Moving to safe_mode" << endl;
					act->low_lev();
					act->low_sta();
					act->off_sta();
					act->off_lev();

					smp = status_message_ptr(new status_message(STATUS_CONTROL,"S0"));
                    fsm_status_queue.push(smp); 
					smp = status_message_ptr(new status_message(STATUS_CONTROL,"L0"));
                    fsm_status_queue.push(smp);
					state = SAFE_MODE_STATE;
                    smp = status_message_ptr(new status_message(STATUS_STATE, state_names[state]));
				}
			} else if(cp->command_type == BRAKING) {
				if(state != FLIGHT_STATE) {
                    string message = "Only brake from FLIGHT state, pod is in " + state_names[state] + " state.";
					cout << message << endl;
                    smp = status_message_ptr(new status_message(STATUS_ERROR, message));
				} else {
					brake(cp->command_value);
					char tmp[2];
					sprintf(tmp,"%1d", cp->command_value);
					std::string b = "B";
					smp = status_message_ptr(new status_message(STATUS_CONTROL, b+tmp));
				}
			 } else if(cp->command_type == FUNCTIONAL_TEST) {	
				if(state != SAFE_MODE_STATE) {
                    string message = "Return to safe_mode before starting functional_test, pod is in " +  state_names[state] +  " state.";
					cout <<  message << endl;
                    smp = status_message_ptr(new status_message(STATUS_ERROR, message));
				} else {
					cout << "Starting functional test" << endl;
					state = FUNCTIONAL_TEST_STATE;
					smp = status_message_ptr(new status_message(STATUS_STATE, state_names[state]));
                    
				}
			} else if(cp->command_type == FLIGHT) {
				if(state != FUNCTIONAL_TEST_STATE) {
                    string message = "Move to functional tests before flight";
					cout << message << endl;
                    smp = status_message_ptr(new status_message(STATUS_ERROR, message));
				} else {
					cout << "Starting flight, motor control enabled" << endl;
					state = FLIGHT_STATE;
					smp = status_message_ptr(new status_message(STATUS_STATE, state_names[state]));
				}
			} else if(cp->command_type == DISCONNECT) {
				if(state == FLIGHT_STATE){
					// cut the motors, wait for braking
					act->off_lev();
					act->off_sta();
				} else if (state == FUNCTIONAL_TEST_STATE) {
					state = SAFE_MODE_STATE;
				}
			}
        if(smp != NULL)
		    fsm_status_queue.push(smp);
		
		}
	}

	printf("Stopping FSM\n");
}

void sensor_loop(void){
    printf("Running Sensor Loop\n");
    //infinate loop is not good idea. come up with something else
    while(1){
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
	//TODO handle connection drops
}

int main(int argc, char ** argv)
{    
    //Initialize the sensors
	if(argc == 1 && !strcmp(argv[0], "test")){
		cout << "Activating test mode" << endl;
		sen = new sensor(&tmp_status_buff, true);
	} else {
    	sen = new sensor(&tmp_status_buff, false);
	}
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
