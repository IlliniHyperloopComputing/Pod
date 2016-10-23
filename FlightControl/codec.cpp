#include "codec.h"

user_select_ptr codec::decode_input(const std::string & x){
    user_select_ptr ep;
    //ensure that size == 6, and first letters are FSM
    if( (x.size()==6) & (x.substr(0,3)=="FSM")){
        std::string state = x.substr(3,3);
        if(state == "SMD")
             ep = user_select_ptr(new user_select(SAFE_MODE));
        else if(state == "ISN")
             ep = user_select_ptr(new user_select(INIT_SENSORS));
        else if(state == "EMB")
             ep = user_select_ptr(new user_select(E_BRAKE));
        else if(state == "FTA")
             ep = user_select_ptr(new user_select(FUNCT_A));
        else if(state == "FTB")
             ep = user_select_ptr(new user_select(FUNCT_B));
        else if(state == "FTC")
             ep = user_select_ptr(new user_select(FUNCT_C));
        else if(state == "FTD")
             ep = user_select_ptr(new user_select(FUNCT_D));
        else if(state == "LDG")
             ep = user_select_ptr(new user_select(LOADING));
        else if(state == "FA1")
             ep = user_select_ptr(new user_select(FLIGHT_A));
        }
    else {
        ep = user_select_ptr(new user_select(SAFE_MODE));
    }
    return ep;
}


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

void codec::create_message(sensor * sen, std::string & buff){
    //clear string of any data
    buff.clear();
    //always add these
    codec::append_to_data_buffer(buff,"XPO",sen->get_atomic_x(),1);
    codec::append_to_data_buffer(buff,"ACC",sen->get_atomic_a(),3);
    codec::append_to_data_buffer(buff,"BRK",sen->get_atomic_brake() ,1);

    codec::append_to_data_buffer(buff,"VEL",sen->get_atomic_v(),3);
    codec::append_to_data_buffer(buff,"HOF",sen->get_atomic_z(),1);
    codec::append_to_data_buffer(buff,"LVH",sen->get_atomic_lev(),2);
    codec::append_to_data_buffer(buff,"ESC",sen->get_atomic_esc(),4);
    codec::append_to_data_buffer(buff,"TOT",sen->get_atomic_tot(),4);

    //for now, lets just send everything. We will see how that works
    /*switch(tick){
        case 3:
            codec::append_to_data_buffer(buff,"VEL",get_v(),3);
            codec::append_to_data_buffer(buff,"HOF",get_z(),1);
            break;
        case 1:
            codec::append_to_data_buffer(buff,"LVH",get_lev(),2);
            codec::append_to_data_buffer(buff,"ESC",get_esc(),4);
            codec::append_to_data_buffer(buff,"ATT",get_att(),3);
            break;
        case 2:
            codec::append_to_data_buffer(buff,"VEL",get_v(),3);
            codec::append_to_data_buffer(buff,"TOT",get_tot(),4);
            break;
        default:
            break;
    }
    */

}
