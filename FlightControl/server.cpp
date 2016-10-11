#include "server.h"

typedef boost::shared_ptr<tcp_connection> pointer;

////////////////
////////////////
////////////////
//tcp_connection
pointer tcp_connection::create(boost::asio::io_service & io_service){
    return pointer(new tcp_connection(io_service, true));
}

tcp::socket& tcp_connection::socket(){
    return socket_;
}

void tcp_connection::start(){

    //create message here-- must be class variable/persist
    message_ = "wow";
    
    //begin writing data
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    //Wait for commands
    boost::asio::async_read_until(socket_,read_buffer_, "\n",
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

}

tcp_connection::tcp_connection(boost::asio::io_service& io_service, bool no_delay)
            : socket_(io_service), no_delay_(no_delay)
    {}
void tcp_connection::handle_write(const boost::system::error_code&,size_t){
    //get data and then write again
    
    
}

void tcp_connection::handle_read(const boost::system::error_code&, std::size_t bytes_transferred){
    //
    

}

std::string tcp_connection::buffer_to_string(boost::asio::streambuf& read_buffer){
    std::string line(
            boost::asio::buffers_begin(read_buffer.data()),
            boost::asio::buffers_end(read_buffer.data()));
    return line;
}


////////////////
////////////////
////////////////
////////////////
//tcp_server
tcp_server::tcp_server(boost::asio::io_service& io_service)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(),8800))
{
    start_accept();//start accepting connections
}

//async wait for a new connection
void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = 
        tcp_connection::create(acceptor_.get_io_service());
    
    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
            boost::asio::placeholders::error));
}

//accept the connection, and if no error, begin sending/recieving data
void tcp_server::handle_accept(tcp_connection::pointer new_connection, 
        const boost::system::error_code& error)
{
    if (! error){
        new_connection->start();
    }
    
    start_accept();//optionally look for another connection

}


