CXXNORMAL=g++
CXXCROSS=arm-linux-gnueabihf-g++

LANGUAGE = -std=c++0x -pedantic
WARNINGS = -Wall -Wextra -Winit-self -Wcast-align -Wcast-qual -Wold-style-cast -Wuninitialized -Wmissing-declarations -Wno-unused-parameter -Wno-unused-variable
DEP=-MMD -MF $(basename $@).dep
OPTIMF= -O2 -ftree-vectorize -fstrict-aliasing -freorder-blocks
OPTIMM= -march=native
OPTIMMBBB= -march=armv7-a

CXXFLAGS = $(LANGUAGE) $(WARNINGS) $(DEP) -g -c
CXXFLAGSO = $(LANGUAGE) $(WARNINGS) $(OPTIMF) $(DEP) -MMD -c

EXECUTABLEBBB=bbb
EXECUTABLEBBBO=bbb-optim
EXECUTABLETEST=test
EXECUTABLETESTO=test-optim
EXECUTABLECROSS=cross

LDFLAGS= -lboost_thread -lboost_system -L/usr/arm-linux-gnueabihf/ -lpthread

all :
	echo "\n================\n================\nuse: make [compiletype]\n\t[compiletype] = 'bbb'\t'bbb-optim' to compile when on the BBB, the -optim option will take a while\n\t[compiletype] = 'test'\t'test-optim' when testing on regular linux machine\n\t[compiletype] = 'cross'\twhen cross compiling to BBB\n================\n================\n"


###############
#BBB/Test compile
###############
$(EXECUTABLEBBB) $(EXECUTABLETEST) : fsm-b.o server-b.o codec-b.o motors-b.o brakes-b.o  sensor-b.o blackpwm-b.o blackcore-b.o blackgpio-b.o
	$(CXXNORMAL) $? $(LDFLAGS) -o $@
fsm-b.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
server-b.o: server.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
codec-b.o: codec.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
sensor-b.o: sensor.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
motors-b.o: motors/motors.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
brakes-b.o: brakes/brakes.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
blackpwm-b.o : ../BlackLib/v3_0/BlackPWM/BlackPWM.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
blackgpio-b.o : ../BlackLib/v3_0/BlackGPIO/BlackGPIO.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@
blackcore-b.o : ../BlackLib/v3_0/BlackCore.cpp
	$(CXXNORMAL) $? $(CXXFLAGS) -o $@

###############
#BBBO compile
###############
$(EXECUTABLEBBBO) : fsm-bo.o server-bo.o codec-bo.o motors-bo.o sensor-bo.o blackpwm-bo.o blackcore-bo.o
	$(CXXNORMAL) $? $(LDFLAGS) -o $@
fsm-bo.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
server-bo.o: server.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
codec-bo.o: codec.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
sensor-bo.o: sensor.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
motors-bo.o: motors/motors.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
blackpwm-bo.o : ../BlackLib/v3_0/BlackPWM/BlackPWM.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@
blackcore-bo.o : ../BlackLib/v3_0/BlackCore.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMBBB) -o $@

###############
#TestO compile
###############
$(EXECUTABLETESTO) : fsm-to.o server-to.o codec-to.o sensor-to.o motors-to.o blackpwm-to.o blackcore-to.o blackgpio-to.o
	$(CXXNORMAL) $? $(LDFLAGS) -o $@
fsm-to.o: fsm.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
server-to.o: server.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
codec-to.o: codec.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
sensor-to.o: sensor.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
motors-to.o: motors/motors.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
blackpwm-to.o : ../BlackLib/v3_0/BlackPWM/BlackPWM.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
blackgpio-to.o : ../BlackLib/v3_0/BlackGPIO/BlackGPIO.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@
blackcore-to.o : ../BlackLib/v3_0/BlackCore.cpp
	$(CXXNORMAL) $? $(CXXFLAGSO) $(OPTIMM) -o $@


##############
#Cross compile
##############
$(EXECUTABLECROSS): fsm-co.o server-co.o codec-co.o sensor-co.o motors-co.o blackpwm-co.o blackcore-co.o brakes-co.o blackgpio-co.o
	$(CXXCROSS) $? $(LDFLAGS) -o $@
fsm-co.o: fsm.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
server-co.o: server.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
codec-co.o: codec.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
sensor-co.o: sensor.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
motors-co.o: motors/motors.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
blackpwm-co.o : ../BlackLib/v3_0/BlackPWM/BlackPWM.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
blackgpio-co.o : ../BlackLib/v3_0/BlackGPIO/BlackGPIO.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
blackcore-co.o : ../BlackLib/v3_0/BlackCore.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@
brakes-co.o: brakes/brakes.cpp
	$(CXXCROSS) $? $(CXXFLAGS) $(OPTIMBBB) -o $@

push:
	scp $(EXECUTABLECROSS) root@192.168.7.2:~/

clean:
	rm -f fsm*.dep black*.dep fsm*.o server*.dep server*.o codec*.dep codec*.o motors*.dep motors*.o black*.o $(EXECUTABLECROSS) $(EXECUTABLETESTO) $(EXECUTABLETEST) $(EXECUTABLEBBB) $(EXECUTABLEBBBO)

