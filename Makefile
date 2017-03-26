all: spidev.o BlackCore.o BlackSPI.o
	g++ $^ -o test
	gcc crc.c spi.c -o spi
spidev.o: spidev.cpp
	g++ -c -std=c++0x $^
BlackCore.o: BlackLib/v3_0/BlackCore.cpp
	g++ -c -std=c++0x $^
BlackSPI.o: BlackLib/v3_0/BlackSPI/BlackSPI.cpp
	g++ -c -std=c++0x $^
