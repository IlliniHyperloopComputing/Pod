#ifndef STATUSQUEUECLASS
#define STATUSQUEUECLASS
#include <string>

enum status_type{
    STATUS_ERROR = 0,
    STATUS_MESSAGE = 1,
    STATUS_CONTROL = 2,
    STATUS_STATE = 3
};
struct status_message {

    status_message(enum status_type type, std::string message) 
        : type(type), message(message)
    {}

    status_message(int state, std::string message) 
        : message(message)
    {
        if(state<0){
            type = STATUS_ERROR;
        }else{
            type = STATUS_MESSAGE;
        }
    
    }
    
    std::string get_message(){
        char prefix = 'E';
        if(type == 1){
            prefix = 'M';
        }else if(type == 2){
            prefix = 'C';
        }else if(type == 3){
            prefix = 'S';
        }
        return prefix + message;
    }
    enum status_type type;
    std::string message;
};
typedef boost::shared_ptr<status_message> status_message_ptr;
typedef boost::lockfree::spsc_queue<status_message_ptr, boost::lockfree::capacity<1024> > status_queue;

#endif
