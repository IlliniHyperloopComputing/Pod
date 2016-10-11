CXX=arm-linux-gnueabihf-g++
CXXNORMAL=g++

LANGUAGE = -std=c++0x -ansi -pedantic
WARNINGS = -Wall -Wextra -Winit-self -Wcast-align -Wcast-qual -Wold-style-cast -Wuninitialized -Wmissing-declarations -Wno-unused-parameter -Wno-unused-variable
OPTIMF= -O2 -ftree-vectorize -fstrict-aliasing -freorder-blocks
OPTIMM= -march=native
OPTIMMCROSS= -march=armv7-a

CXXFLAGS = $(LANGUAGE) $(WARNINGS) $(OPTIMF) -MMD -c


EXECUTABLENORMAL=test
EXECUTABLE=test_cross

LDFLAGS= -lboost_thread -lboost_system

all: $(EXECUTABLENORMAL) $(EXECUTABLE)

###############
#Normal compile
###############
$(EXECUTABLENORMAL): fsm-n.o server-n.o
	$(CXXNORMAL) $? $(LDFLAGS) -o $@

fsm-n.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) $(OPTIMM) -o $@

server-n.o: server.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) $(OPTIMM) -o $@



##############
#Cross compile
##############
$(EXECUTABLE): fsm-c.o 
	$(CXX) $? $(LDFLAGS) -o $@

fsm-c.o: fsm.cpp
	$(CXX) $? $(CXXFLAGS) $(OPTIMMCROSS) -o $@

server-c.o: server.cpp
	$(CXX) $? $(CXXFLAGS) $(OPTIMMCROSS) -o $@

push:
	scp $(EXECUTABLE) root@192.168.7.2:~/


clean:
	rm -f fsm-n.o fsm-c.o server-n.o server-c.o $(EXECUTABLE) $(EXECUTABLENORMAL)

