#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include <string>

using boost::asio::ip::tcp;
using namespace std;

enum { max_length = 1024 };
double data[] = {20.0,5.1,-1.1}; //simulating attitude data
double data_size = 3; //size of data[] array

string makeDataString(){	
	string result = "";

	for(int i = 0; i < data_size; i++){//for each element in data[] array
		char tmp[21];
		sprintf(tmp,"%F", data[i]); //print uppercase decimal floating point data
		result += tmp; //concactinating result and data[i]
		result += ","; //deliniate
	} 

	result = result.substr(0,result.length()-1);//remove last unecessary comma
	result = "ATT" + result; //type of data. in this case, attitude

	size_t size = result.length();//size of the "data"
	char len[5];
	sprintf(len,"%04d", size);//4 character size
	result = len + result; //add the length of the data in front

	return result;
}


void updateData(){
	double ran =(rand()%10-5)/100.0;

	data[0] += .010;
	data[1] += ran;
	data[2] += ran;
}

int main(int argc, char* argv[])
{
  try
  {
	std::cout << argc << std::endl;
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }
    boost::asio::ip::tcp::no_delay option(true);
    boost::asio::io_service io_service;

    tcp::socket s(io_service);
    tcp::resolver resolver(io_service);
    boost::asio::connect(s, resolver.resolve({argv[1], argv[2]}));

    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    for(int i = 0; i<1000; i++){
	    string toSend = makeDataString();
	    
	    size_t request_length = toSend.length();
	    boost::asio::write(s, boost::asio::buffer(toSend, request_length));
	    updateData();

	    char reply[max_length];
	    size_t reply_length = boost::asio::read(s,
		boost::asio::buffer(reply,9));
	    std::cout << "Reply is: ";
	    std::cout.write(reply, reply_length);
	    std::cout << "\n";
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
