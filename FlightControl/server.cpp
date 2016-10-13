#include "server.h"
typedef boost::shared_ptr<tcp_connection> pointer;
typedef boost::shared_ptr<user_select> user_select_ptr;
typedef boost::lockfree::spsc_queue<user_select_ptr, boost::lockfree::capacity<1024> > user_queue;

////////////////
////////////////
////////////////
//tcp_connection
pointer tcp_connection::create(boost::asio::io_service & io_service, user_queue * queue){
    std::cout<<"we were able to create a new pointer"<<std::endl;
    return pointer(new tcp_connection(io_service, true, queue));
}

tcp::socket& tcp_connection::socket(){
    return socket_;
}

void tcp_connection::start(){
    //create message here-- must be class variable/persist
    message_ = "wow";
    
    //begin writing data
    std::cout<<"writing data now"<<std::endl;
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    std::cout<<"reading data now"<<std::endl;
    //Wait for commands
    boost::asio::async_read(socket_,read_buffer_.prepare(6),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    read_buffer_.commit(6);

}

tcp_connection::tcp_connection(boost::asio::io_service& io_service, bool no_delay, user_queue * queue)
            : socket_(io_service), no_delay_(no_delay), queue_(queue)
{ 

}
void tcp_connection::handle_write(const boost::system::error_code&,size_t bytes_transferred){
    //get data and then write again
/*    boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
*/
}

void tcp_connection::handle_read(const boost::system::error_code&, std::size_t bytes_transferred){
    std::cout<<"reading data now, bytes_transfered: "<<bytes_transferred<<std::endl;
    //get string value of read
    std::string buf_val = buffer_to_string(read_buffer_,bytes_transferred);
    //parse read value using decoder_input()
    user_select_ptr usp = codec::decode_input(buf_val);
    queue_->push(usp);
    std::cout<<"value: "<<buf_val<<std::endl;
    //read again
    boost::asio::async_read(socket_,read_buffer_.prepare(6),
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    read_buffer_.commit(6);

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
tcp_server::tcp_server(boost::asio::io_service& io_service, user_queue *queue)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(),8888)), queue_(queue)
{
    start_accept();//start accepting connections
}

//async wait for a new connection
void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = 
        tcp_connection::create(acceptor_.get_io_service(),queue_);


    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
            boost::asio::placeholders::error));
}

//accept the connection, and if no error, begin sending/recieving data
void tcp_server::handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
{
    if (! error){
        std::cout<<"got new connection!"<<std::endl;
        boost::asio::ip::tcp::no_delay option(true);
        new_connection->socket().set_option(option);
        new_connection->start();
    }
    
    start_accept();//optionally look for another connection

}


