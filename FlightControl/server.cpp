#include "server.h"

////////////////
////////////////
////////////////
//tcp_connection
static pointer tcp_connection::create(boost::asio::io_service & io_service){
    return pointer(new tcp_connection(io_service));
}

tcp::socket& tcp_connection::socket(){
    return socket_;
}

void tcp_connection::start(){

    //create message here-- must be class variable/persist
    std::string message_;
    message_ = "wow"
    
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
            boost::bind(&tcp_connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

tcp_connection::tcp_connection(boost::asio::io_service& io_service)
            : socket_(io_service)
    {}
void tcp_connection::handle_write(const boost::system::error_code&,size_t){
    //handle that write, yo

    //get data and then write again
}


////////////////
////////////////
////////////////
////////////////
//tcp_server
tcp_server::tcp_server(boost::asio::io_service& io_service)
            : acceptor_(io_service, tcp::endpoint(tcp::v4(),8800))
{
    start_accept();
}

void tcp_server::start_accept()
{
    tcp_connection::pointer new_connection = 
        tcp_connection::create(acceptor_.get_io_service());
    
    acceptor_.async_accept(new_connection->socket(),
        boost::bind(&tcp_server::handle_accept, this, new_connection,
            boost::asio::placeholders::error));
}

void tcp_server::handle_accept(tcp_connection::pointer new_connection, 
        const boost::system::error_code& error)
{
    if (! error){
        new_connection->start();
    }
    
    start_accept();//optionally look for another connection

}


