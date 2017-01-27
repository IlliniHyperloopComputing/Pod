#ifndef ASYNCTCPSERVER
#define ASYNCTCPSERVER

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <boost/asio.hpp>
#include "codec.h"
#include "sensor.h"
#include "status.h"

using boost::asio::ip::tcp;
typedef boost::shared_ptr<command> command_ptr;
typedef boost::lockfree::spsc_queue<command_ptr, boost::lockfree::capacity<1024> > command_queue;

class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
    public:
        typedef boost::shared_ptr<tcp_connection> pointer;
        static pointer create(boost::asio::io_service& io_service, command_queue * queue, sensor * sen, status_queue * fsm_status_queue);
        tcp::socket& socket();
        void start();
    
    private:
        tcp_connection(boost::asio::io_service& io_service,bool no_delay, command_queue *queue, sensor * sen, status_queue *fsm_status_queue);
        void handle_write(const boost::system::error_code& , size_t);
        void handle_read(const boost::system::error_code&, size_t bytes_transferred);
        std::string buffer_to_string(boost::asio::streambuf& read_buffer, std::size_t bytes_transferred);

        tcp::socket socket_;
        tcp::no_delay no_delay_;
        std::string message_;
        boost::asio::streambuf read_buffer_;
        command_queue * queue_;
        sensor * sensor_;
        status_queue * fsm_status_queue_;
};

class tcp_server{
    public:
        tcp_server(boost::asio::io_service& io_service, command_queue * queue, status_queue * fsm_status_queue, sensor * sen);
        ~tcp_server();
    
    private:
        void start_accept();
        void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code&error);

        tcp::acceptor acceptor_;
        command_queue * incoming_command_queue_;
        status_queue * fsm_status_queue_;
        sensor  * sensor_;
};

#endif
