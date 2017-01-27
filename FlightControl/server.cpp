#include "server.h"
typedef boost::shared_ptr<tcp_connection> pointer;
//typedef boost::shared_ptr<command> command_ptr;
//typedef boost::lockfree::spsc_queue<command_ptr, boost::lockfree::capacity<1024> > user_queue;
#define COMMAND_SIZE 7

////////////////
////////////////
////////////////
//tcp_connection
pointer tcp_connection::create(boost::asio::io_service & io_service, command_queue* queue, sensor * sen, status_queue * fsm_status_queue){
    return pointer(new tcp_connection(io_service, true, queue, sen, fsm_status_queue));
}

tcp::socket& tcp_connection::socket(){
    return socket_;
}

void tcp_connection::start(){
    //create message here-- must be class variable/persist
    codec::create_message(sensor_,fsm_status_queue_,message_);//create message to send
    std::cout<<"inside start tcp con"<<std::endl;
    
    //begin writing data
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    std::cout<<"just wrote some data"<<std::endl;

    //Wait for commands
    boost::asio::async_read(socket_,read_buffer_.prepare(COMMAND_SIZE),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    read_buffer_.commit(COMMAND_SIZE);
    std::cout<<"just wrote read some data"<<std::endl;

}

tcp_connection::tcp_connection(boost::asio::io_service& io_service, bool no_delay, command_queue* queue, sensor * sen, status_queue * fsm_status_queue)
            : socket_(io_service), no_delay_(no_delay), queue_(queue), sensor_(sen), fsm_status_queue_(fsm_status_queue)
{ 

}
void tcp_connection::handle_write(const boost::system::error_code& error_message,size_t bytes_transferred){

    usleep(3000);
    if(socket_.is_open()){
        codec::create_message(sensor_,fsm_status_queue_,message_);//create message to send

        //send data
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
                boost::bind(&tcp_connection::handle_write, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

}

void tcp_connection::handle_read(const boost::system::error_code& error_message, std::size_t bytes_transferred){
    if(!error_message){
        //get string value of read
        std::string buf_val = buffer_to_string(read_buffer_,bytes_transferred);
        //parse read value using decoder_input()
        command_ptr cp = codec::decode_input(buf_val);
        queue_->push(cp);
        //read again if no error
        boost::asio::async_read(socket_,read_buffer_.prepare(COMMAND_SIZE),
                boost::bind(&tcp_connection::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
        read_buffer_.commit(COMMAND_SIZE);
    }
    else if(boost::asio::error::eof == error_message || boost::asio::error::connection_reset == error_message){
        std::cout << error_message.message() << std::endl;
        boost::system::error_code errorcode;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_type::shutdown_both, errorcode);
        if(errorcode){
            std::cout<<"socket.shutdown error: "<<errorcode.message() <<std::endl;
        }
        socket_.close(errorcode);
        if(errorcode){
            std::cout<<"socket.close error: "<<errorcode.message() <<std::endl;
        }
        command_ptr c= command_ptr(new command(DISCONNECT));
        queue_->push(c);
    }

}

std::string tcp_connection::buffer_to_string(boost::asio::streambuf& read_buffer, std::size_t bytes_transferred){
    std::string line(
            boost::asio::buffers_begin(read_buffer.data()),
            boost::asio::buffers_end(read_buffer.data()));
    read_buffer_.consume(bytes_transferred);
    return line;
}


////////////////
////////////////
////////////////
////////////////
//tcp_server
//typedef  spsc_queue;
tcp_server::tcp_server(boost::asio::io_service& io_service, command_queue *incoming_command_queue, status_queue * fsm_status_queue, sensor * sen)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(),8888)), incoming_command_queue_(incoming_command_queue), fsm_status_queue_(fsm_status_queue), sensor_(sen) 
{
    start_accept();//start accepting connections
}

tcp_server::~tcp_server()
{
}

//async wait for a new connection
void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = 
        tcp_connection::create(acceptor_.get_io_service(),incoming_command_queue_, sensor_, fsm_status_queue_);


    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
            boost::asio::placeholders::error));
}

//accept the connection, and if no error, begin sending/recieving data
void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
{
    if (! error){
        std::cout<<"New Connection:"<<std::endl;
        boost::asio::ip::tcp::no_delay option(true);
        new_connection->socket().set_option(option);
        std::cout<<"About to start new connection"<<std::endl;
        new_connection->start();
    }
    start_accept();//optionally look for another connection
}

