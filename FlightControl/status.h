#ifndef STATUSQUEUECLASS
#define STATUSQUEUECLASS
#include <string>

enum status_type{
    STATUS_ERROR = 0,
    STATUS_MESSAGE = 1,
    STATUS_STATE = 2
};
struct status_message {
    status_message()
        : type(STATUS_MESSAGE), message("")
    {}

    status_message(enum status_type type, std::string message) 
        : type(type), message(message)
    {}
    
    std::string operator()(){
        char prefix = 'e';
        if(type == 1){
            prefix = 'm';
        }else if(type == 2){
            prefix = 's';
        }
        return prefix + message;
    }
    enum status_type type;
    std::string message;
};

typedef boost::shared_ptr<status_message> status_message_ptr;
typedef boost::lockfree::spsc_queue<status_message_ptr, boost::lockfree::capacity<512> > status_queue;

class status{
    public:
        status(status_queue * fsm_queue, status_queue * sensor_queue);
        std::string get_status();
        void push(enum status_type type, std::string message);
    private:
        status_queue * fsm_queue;
        status_queue * sensor_queue;

};

#endif
