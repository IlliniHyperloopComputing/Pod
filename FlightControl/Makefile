CXX=arm-linux-gnueabihf-g++
CXXNORMAL=g++

CXXFLAGS=-std=c++0x -O0 -g3 -Wall -c

EXECUTABLE=test_build_cross
EXECUTABLENORMAL=test_build

LDFLAGS= -lboost_thread -lboost_system

cross-compile: $(EXECUTABLE)

$(EXECUTABLE): fsm.o
	$(CXX) $? $(LDFLAGS) -o $@

fsm.o: fsm.cpp
	$(CXX) $(CXXFLAGS) $? -o $@

push:
	scp $(EXECUTABLE) root@192.168.7.2:~/

normal: $(EXECUTABLENORMAL)

$(EXECUTABLENORMAL): fsm-n.o
	$(CXXNORMAL) $? $(LDFLAGS) -o $@

fsm-n.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@



clean:
	rm -f fsm-n.o fsm.o events.o $(EXECUTABLE) $(EXECUTABLENORMAL)

