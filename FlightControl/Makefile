CXXNORMAL=g++
CXXCROSS=arm-linux-gnueabihf-g++

LANGUAGE = -std=c++0x -ansi -pedantic
WARNINGS = -Wall -Wextra -Winit-self -Wcast-align -Wcast-qual -Wold-style-cast -Wuninitialized -Wmissing-declarations -Wno-unused-parameter -Wno-unused-variable
DEP=-MMD -MF $(basename $@).dep
OPTIMF= -O2 -ftree-vectorize -fstrict-aliasing -freorder-blocks
OPTIMM= -march=native
OPTIMMBBB= -march=armv7-a

CXXFLAGS = $(LANGUAGE) $(WARNINGS) $(DEP) -Og -c
CXXFLAGSO = $(LANGUAGE) $(WARNINGS) $(OPTIMF) $(DEP) -MMD -c

EXECUTABLEBBB=bbb
EXECUTABLEBBBO=bbb-optim
ECECUTABLETEST=test
ECECUTABLETESTO=test-optim
EXECUTABLECROSS=cross

LDFLAGS= -lboost_thread -lboost_system

all :
	echo "\n================\n================\nuse: make [compiletype]\n\t[compiletype] = 'bbb'\t'bbb-optim' to compile when on the BBB, the -optim option will take a while\n\t[compiletype] = 'test'\t'test-optim' when testing on regular linux machine\n\t[compiletype] = 'cross'\twhen cross compiling to BBB\n================\n================\n"


###############
#BBB/Test compile
###############
$(EXECUTABLEBBB) $(EXECUTABLETEST) : fsm-b.o server-b.o codec-b.o motors-b.o 
	$(CXXNORMAL) $? $(LDFLAGS) -o $@
fsm-b.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
server-b.o: server.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
codec-b.o: codec.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
motors-b.o: motors/motors.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@

###############
#BBBO compile
###############
$(EXECUTABLEBBBO) : fsm-bo.o server-bo.o codec-bo.o motors-bo.o 
	$(CXXNORMAL) $? $(LDFLAGS) -o $@
fsm-bo.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
server-bo.o: server.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
codec-bo.o: codec.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
motors-bo.o: motors/motors.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@

###############
#Test compile
###############
#$(EXECUTABLETEST) : fsm-t.o server-t.o codec-t.o motors-t.o 
#	$(CXXNORMAL) $? $(LDFLAGS) -o $@
#fsm-t.o: fsm.cpp
#	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
#server-t.o: server.cpp
#	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
#codec-t.o: codec.cpp
#	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
#motors-t.o: motors/motors.cpp
#	$(CXXNORMAL) $? $(CXXFLAGS) -o $@

###############
#TestO compile
###############
$(EXECUTABLETESTO) : fsm-to.o server-to.o codec-to.o motors-to.o 
	$(CXXNORMAL) $? $(LDFLAGS) -o $@
fsm-to.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
server-to.o: server.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
codec-to.o: codec.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
motors-to.o: motors/motors.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@


##############
#Cross compile
##############
$(EXECUTABLECROSS): fsm-co.o server-co.o codec-co.o motors-co.o 
	$(CXXCROSS) $? $(LDFLAGS) -o $@
fsm-co.o: fsm.cpp
	$(CXXCROSS) $? $(CXXFLAGSO) $(OPTIMM) -o $@
server-co.o: server.cpp
	$(CXXCROSS) $? $(CXXFLAGSO) $(OPTIMM) -o $@
codec-co.o: codec.cpp
	$(CXXCROSS) $? $(CXXFLAGSO) $(OPTIMM) -o $@
motors-co.o: motors/motors.cpp
	$(CXXCROSS) $? $(CXXFLAGSO) $(OPTIMM) -o $@

push:
	scp $(EXECUTABLE) root@192.168.7.2:~/

clean:
	rm -f fsm*.o server*.o codec*.o motors*.o $(EXECUTABLECROSS) $(EXECUTABLETESTO) $(EXECUTABLETEST) $(EXECUTABLEBBB) $(EXECUTABLEBBBO)

