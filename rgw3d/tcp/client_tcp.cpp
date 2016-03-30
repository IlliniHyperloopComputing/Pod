#include "client_tcp.h"

client_tcp::client_tcp()
	:socket(io_service), resolver(io_service), no_delay(true){
}

client_tcp::client_tcp(char * ip, char * port)
	:ip(ip), port(port), socket(io_service), resolver(io_service), no_delay(true){
	try{
		boost::asio::connect(socket, resolver.resolve({ip, port})); //connect to specified ip:port
		socket.set_option(no_delay);//disable the Nagle algorithm 
		//TODO: Ensure that disabling the Nagle algorithm is the right choice. I think it is 
	}catch (exception& e){
		cerr << "Error during connection (constructor) Make sure ip and port are valid"<< endl;
		cerr << e.what() <<endl;
		throw e;
	}
}


//ALL SEND FUNCTIONS ARE BLOCKING
void client_tcp::send_KIN(double *data){
	send(make_data_string(KIN, data));
}
void client_tcp::send_ATT(double *data){
	send(make_data_string(ATT, data));
}
void client_tcp::send_TMP(double *data){
	send(make_data_string(TMP, data));
}
void client_tcp::send_BAT(double *data){
	send(make_data_string(BAT, data));
}
void client_tcp::send_CBP(double *data){
	send(make_data_string(CBP, data));
}

//BLOCKING
void client_tcp::send(const string& to_send){
	size_t send_length = to_send.length();
	boost::asio::write(this->socket, boost::asio::buffer(to_send, send_length));
}

string client_tcp::make_data_string(enum message_type type, double *data){
	uint8_t data_size = 0;//size of the "data" array
	string msg_type = "";//type of message
	
	switch(type){//initialize data_size and msg_type
		case ALL:
			data_size = 18;
			msg_type = "ALL";
			break;
		case KIN:
			data_size = 9;
			msg_type = "KIN";
			break;
		case ATT:
			data_size = 3;
			msg_type = "ATT";
			break;
		case TMP:
			data_size = 2;
			msg_type = "TMP";
			break;
		case BAT:
			data_size = 3;
			msg_type = "BAT";
			break;
		case CBP:
			data_size= 1;
			msg_type = "CBP";
			break;
	}
	
	string result = "";	
	result.reserve(200);//store room in result now
	for(int i = 0; i<data_size; i++){
		char tmp[21];
		sprintf(tmp,"%F", data[i]);
		result += tmp;
		result += ",";
	}
	size_t message_size = result.length();//get the size of the message
	result = result.substr(0, message_size-1);//removes last comma
	message_size--;//decrement message size since we just removed a comma

	char tmp[5];
	sprintf(tmp,"%04d", message_size);//copy message size 

	return tmp + msg_type + result;//adds size, message type, and the data
}

//BLOCKING
//TODO: actually use Boost.ASIO to its potential!
string client_tcp::read(){
	const int read_size = 9;
	char reply[read_size+1];
	
	size_t reply_length = boost::asio::read(this->socket, boost::asio::buffer(reply,read_size));
	string reply_string = "";
	reply_string +=reply;
	reply_string = reply_string.substr(0,read_size);
	return reply_string;
	//reads a default amount of bytes
}
