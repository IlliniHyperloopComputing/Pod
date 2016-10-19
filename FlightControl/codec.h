#ifndef CODEC_FOR_IO_PARSING
#define CODEC_FOR_IO_PARSING
//Used to parse communication input 
//Used to create communication output
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include "events.hpp"
typedef boost::shared_ptr<user_select> user_select_ptr;

class codec{
    public:
        codec();
        static user_select_ptr decode_input(const std::string & x);
        void create_message(std::string & buff);
    private:
        void append_to_data_buffer(string & type, double *data, int size);
};
        

#endif
