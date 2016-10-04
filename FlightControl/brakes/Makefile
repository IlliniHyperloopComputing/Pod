CXX=g++


CXXFLAGS=-std=c++0x -O0 -g3 -Wall -c -pthread 

LDFLAGS=-lpthread

#SOURCES=./BlackADC/BlackADC.cpp ./BlackDirectory/BlackDirectory.cpp  ./BlackGPIO/BlackGPIO.cpp ./BlackI2C/BlackI2C.cpp ./BlackMutex/BlackMutex.cpp  ./BlackSPI/BlackSPI.cpp ./BlackThread/BlackThread.cpp ./BlackTime/BlackTime.cpp  ./BlackUART/BlackUART.cpp  ./examples.cpp

EXECUTABLE=test_build

all: $(EXECUTABLE)
    
$(EXECUTABLE): blackgpio.o main.o brakes.o blackcore.o
	$(CXX) $? $(LDFLAGS) -o $@

main.o : main_test.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

brakes.o : brakes.cpp 
	$(CXX) $(CXXFLAGS) $? -o $@

blackgpio.o : ../../BlackLib/v3_0/BlackGPIO/BlackGPIO.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

blackcore.o : ../../BlackLib/v3_0/BlackCore.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

clean:
	rm -f brakes.o
	rm -f main.o
	rm -f blackgpio.o
