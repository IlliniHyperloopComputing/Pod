#include "codec.h"

command_ptr codec::decode_input(const std::string & x){
    command_ptr ep;
        //commands of the form LEV1000, FA1
        std::string state = x.substr(0,3);
        //std::cout << "Command string = " << x << std::endl; 
        if(state == "SMD")
             ep = command_ptr(new command(SAFE_MODE));
        else if(state == "BRK")
             ep = command_ptr(new command(BRAKING));
        else if(state == "FTA")
             ep = command_ptr(new command(FUNCTIONAL_TEST));
        else if(state == "FLT")
             ep = command_ptr(new command(FLIGHT));
        else if(state == "LEV"){
             int value = stoi(x.substr(3));
             ep = command_ptr(new command(LEV_MOTOR, value));
        }
        else if(state == "STA"){
             int value = stoi(x.substr(3));
             ep = command_ptr(new command(STA_MOTOR, value)); 
        }
        else if(state == "ARL"){//arm lev
             int value = stoi(x.substr(3));
             ep = command_ptr(new command(ARM_LEV_MOTOR, value)); 
        }
        else if(state == "ARS"){//arm sta
             int value = stoi(x.substr(3));
             ep = command_ptr(new command(ARM_STA_MOTOR, value)); 
        }
        else {
			cout << "Unregistered command" << endl;
            ep = command_ptr(new command(SAFE_MODE));
        }
    return ep;
}


//append_to_data_buffer for double * data
//different method below for atomics
void codec::append_to_data_buffer(std::string & buff, const std::string & type, double * data, size_t data_length){
    std::string message;
    
    message.reserve((21 * data_length));
    for(size_t i = 0; i<data_length; i++){
        char tmp[21];
        sprintf(tmp, "%F", data[i]);
        message += tmp;
        message += ",";
    }
    //remove last comma and compute size
    int message_size = message.length();
    message = message.substr(0, message_size-1);
    message_size--;

    //calculate message size
	char tmp[5];
	sprintf(tmp,"%04d", message_size);//write message size 


    //reserver extra space to buffer
    buff.reserve(7+message_size+buff.length());
    //add to buffer
    buff+=tmp;
    buff+=type;
    buff+=message;
}

//append_to_data_buffer for atomic<double> * data
void codec::append_to_data_buffer(std::string & buff, const std::string & type, std::atomic<double> const * const data, size_t data_length){
    std::string message;
    
    message.reserve((21 * data_length));
    for(size_t i = 0; i<data_length; i++){
        char tmp[21];
        sprintf(tmp, "%F", data[i].load(std::memory_order_relaxed));
        message += tmp;
        message += ",";
    }
    //remove last comma and compute size
    int message_size = message.length();
    message = message.substr(0, message_size-1);
    message_size--;

    //calculate message size
	char tmp[5];
	sprintf(tmp,"%04d", message_size);//write message size 


    //reserver extra space to buffer
    buff.reserve(7+message_size+buff.length());
    //add to buffer
    buff+=tmp;
    buff+=type;
    buff+=message;
}

void codec::append_to_data_buffer(std::string & buff, const std::string & type, const std::string & data, size_t data_length){

    //calculate message size
    int message_size = data.length();
	char tmp[5];
	sprintf(tmp,"%04d", message_size);//write message size 


    //reserver extra space to buffer
    buff.reserve(7+message_size+buff.length());
    //add to buffer
    buff+=tmp;
    buff+=type;
    buff+=data;
}

void codec::create_message(sensor * sen, status_queue * fsm_status_queue, std::string & buff){
    //clear string of any data
    buff.clear();
    //always add these
    codec::append_to_data_buffer(buff,"XPO",sen->get_distance(),1);
    codec::append_to_data_buffer(buff,"ACC",sen->get_atomic_a(),3);
    codec::append_to_data_buffer(buff,"BRK",sen->get_atomic_brake() ,1);

    codec::append_to_data_buffer(buff,"VEL",sen->get_atomic_v(),3);
    codec::append_to_data_buffer(buff,"LVH",sen->get_atomic_lev(),3);
    codec::append_to_data_buffer(buff,"TMP",sen->get_atomic_temps(), 8);
    codec::append_to_data_buffer(buff,"RPM",sen->get_atomic_rpm(), 6);

    status_message_ptr smp;
    fsm_status_queue->pop(smp);
    if(smp){
        std::string msg = smp->get_message();
        codec::append_to_data_buffer(buff,"STT",msg,msg.length());
    }

    


}
