testGyroReadSend : client_tcp.o test_gyro_read_send.o
	g++ client_tcp.o test_gyro_read_send.o -std=c++0x -pthread -lboost_system -o testGyroReadSend

client_tcp.o : ../tcp/client_tcp.cpp
	g++ ../tcp/client_tcp.cpp -c -std=c++0x -pthread -lboost_system -o client_tcp.o

test_gyro_read_send.o : test_gyro_read_send.cpp
	g++ test_gyro_read_send.cpp -c -std=c++0x -pthread -lboost_system -o test_gyro_read_send.o

clean : 
	rm *.o testGyroReadSend
