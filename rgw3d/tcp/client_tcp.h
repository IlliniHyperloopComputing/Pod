#ifndef CLIENT_TCP
#define CLIENT_TCP

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <string>
using boost::asio::ip::tcp;
using namespace std;

enum message_type{
	FUNCTIONAL_TEST,
	ALL,
	KIN,
	ATT,
	TMP,
	BAT,
	CBP,
};

class client_tcp{
	public:
		client_tcp();
		client_tcp(char * ip, char *port);
		
		void send_KIN(double* data);
		void send_ATT(double* data);
		void send_TMP(double* data);
		void send_BAT(double* data);
		void send_CBP(double* data);

		string read();

	private:
		//functions
		void send(const string& to_send);
		string make_data_string(enum message_type type, double *data);
	
		//data
		char * ip;
		char * port;

		boost::asio::io_service io_service;
		tcp::socket socket;
		tcp::resolver resolver;
		tcp::no_delay no_delay;

};

#endif
