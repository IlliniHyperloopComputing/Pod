testClientTCP : client_tcp.o test_client_tcp.o
	g++ client_tcp.o test_client_tcp.o -std=c++0x -pthread -lboost_system -o testClientTCP

client_tcp.o : client_tcp.cpp
	g++ client_tcp.cpp -c -std=c++0x -pthread -lboost_system -o client_tcp.o

test_client_tcp.o : test_client_tcp.cpp
	g++ test_client_tcp.cpp -c -std=c++0x -pthread -lboost_system -o test_client_tcp.o

clean : 
	rm *.o testClientTCP
