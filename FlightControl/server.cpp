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
    boost::asio::async_read_until(socket_, , "\n",
            boost::bind(&tcp_connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    socket_.async_read_until(boost::asio::buffer(data_
}

tcp_connection::tcp_connection(boost::asio::io_service& io_service)
            : socket_(io_service)
    {}
void tcp_connection::handle_write(const boost::system::error_code&,size_t){
    //handle that write, yo

    //get data and then write again
}

void tcp_connection::handle_read(const boost:system::error_code&, std::size_t bytes_transferred){
    


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


