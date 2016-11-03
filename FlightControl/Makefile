CXXNORMAL=g++
CXXCROSS=arm-linux-gnueabihf-g++

CPP_BLACKLIB := $(addprefix ../BlackLib/v3_0/, BlackGPIO/BlackGPIO.cpp BlackCore.cpp BlackPWM/BlackPWM.cpp)
OBJ_BLACKLIB := $(patsubst %.cpp, obj/%.o, $(notdir $(CPP_BLACKLIB)))

CPP_FILES := $(wildcard *.cpp) ##$(addprefix ../BlackLib/v3_0/, $(CPP_BLACKLIB))
OBJ_FILES := $(patsubst %.cpp,obj/%.o,$(CPP_FILES))

LANGUAGE = -std=c++0x -pedantic
WARNINGS = -Wall -Wextra -Winit-self -Wcast-align -Wcast-qual -Wold-style-cast -Wuninitialized -Wmissing-declarations -Wno-unused-parameter -Wno-unused-variable
DEP= -MMD ##$(basename $@).dep
OPTIM= -O2 -ftree-vectorize -fstrict-aliasing -freorder-blocks -march=armv7-a

CXXFLAGS = $(LANGUAGE) $(WARNINGS) $(DEP) -c

LDFLAGS= -lboost_thread -lboost_system -L/usr/arm-linux-gnueabihf -lpthread

EXECUTABLEBBB=bbb
EXECUTABLECROSS=cross
EXECUTABLECROSSO=crosso

all :
	echo "\n================\n================\nuse: make [compiletype]\n\t[compiletype] = 'bbb'\t'bbb-optim' to compile when on the BBB, the -optim option will take a while\n\t[compiletype] = 'test'\t'test-optim' when testing on regular linux machine\n\t[compiletype] = 'cross'\twhen cross compiling to BBB\n================\n================\n"

$(EXECUTABLEBBB) : CXXFLAGS += -g
$(EXECUTABLEBBB) : CXX = $(CXXNORMAL)
$(EXECUTABLEBBB) : build

$(EXECUTABLECROSS) : CXXFLAGS += -g
$(EXECUTABLECROSS) : CXX = $(CXXCROSS)
$(EXECUTABLECROSS) : build

$(EXECUTABLECROSSO) : CXXFLAGS += $(OPTIM)
$(EXECUTABLECROSSO) : CXX = $(CXXCROSS)
$(EXECUTABLECROSSO) : build

build : $(OBJ_FILES) $(OBJ_BLACKLIB)
	$(CXX) $? $(LDFLAGS) -o $@
obj/%.o : %.cpp 
	$(CXX) $? $(CXXFLAGS) -o $@
obj/BlackPWM.o : ../BlackLib/v3_0/BlackPWM/BlackPWM.cpp
	$(CXX) $? $(CXXFLAGS) -o $@
obj/BlackGPIO.o : ../BlackLib/v3_0/BlackGPIO/BlackGPIO.cpp
	$(CXX) $? $(CXXFLAGS) -o $@
obj/BlackCore.o : ../BlackLib/v3_0/BlackCore.cpp
	$(CXX) $? $(CXXFLAGS) -o $@

push:
	scp $(EXECUTABLECROSS) root@192.168.7.2:~/
	scp $(EXECUTABLECROSS) root@192.168.137.10:~/
	scp $(EXECUTABLECROSS) root@192.168.137.100:~/

clean:
	rm -f obj/*
