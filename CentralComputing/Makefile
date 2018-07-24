# Set up source file list and obj list
OBJ_DIR := .objs
SRC := $(shell find . -name "*.cpp")
SRC := $(patsubst ./%.cpp, %.cpp, $(SRC))
SRC := $(sort $(SRC))
OBJ := $(patsubst %.cpp, %.o, $(SRC))

# Set up path to test library
GTEST_DIR := googletest/googletest
LIB_DIR   := .lib

# Set up compiler
CXX_NORM	:= g++
CXX_BBB 	:= arm-linux-gnueabihf-g++
WARNINGS 	:= -Wall -Wextra -pedantic -Wdouble-promotion -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wno-unused

INCLUDE_DIRS	:= -IStateMachineCompact -I${GTEST_DIR}/include 
CFLAGS_RELEASE 	:= -O2 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DNDEBUG
CFLAGS_DEBUG   	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DDEBUG 
CFLAGS_SIM 	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DDEBUG -DSIM

#Set up linker
LDFLAGS := -pthread -lm -static-libgcc -static-libstdc++ ${LIB_DIR}/libgtest.a
LD_NORM := $(CXX_NORM)
LD_BBB  := $(CXX_BBB)

# Define all executables
POD = build
POD_D = dbuild
POD_T = sbuild
POD_CROSS = cross
POD_CROSS_D = dcross
POD_CROSS_T = scross

.PHONY: all
all : msg mkdir_obj ${LIB_DIR}/gtest-all.o msg_d $(POD_D) msg_s $(POD_T)

# Compile libgtest
${LIB_DIR}/gtest-all.o : ${GTEST_DIR}/src/gtest-all.cc 
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o ${LIB_DIR}/gtest-all.o
	ar -rv ${LIB_DIR}/libgtest.a ${LIB_DIR}/gtest-all.o

.PHONY: msg
msg : 
	$(info =      __________   ___      ___     __________   ___     ___  __________ )
	$(info =     /___   ___/  /  /     /  /    /___   ___/  /   |   /  / /___   ___/ )
	$(info =        /  /     /  /     /  /        /  /     /    |  /  /     /  /     )
	$(info =       /  /     /  /     /  /        /  /     /  /| | /  /     /  /      )
	$(info =      /  /     /  /     /  /        /  /     /  / | |/  /     /  /       )
	$(info =     /  /     /  /     /  /        /  /     /  /  |    /     /  /        )
	$(info = ___/  /___  /  /___  /  /___  ___/  /___  /  /   |   /   __/  /___      )
	$(info =/_________/ /______/ /______/ /_________/ /__/    |__/   /_________/     )
	$(info =       ___    ___  ___    ___  ________    __________  _____________     )  
	$(info =      /  /   /  / /  /   /  / /  ___   \  /  _______/ /  ___   \         )
	$(info =     /  /   /  / /  /   /  / /  /   /  / /  /        /  /   /  /____ _   )             
	$(info =    /  /___/  / /  /___/  / /  /___/__/ /  /___     /  /___/__/          )   
	$(info =   /  ____   / /______/  / /  _____/   /  ____/    /  _____/___ __       )
	$(info =  /  /   /  /        /  / /  /        /  /        /  \  \                ) 
	$(info = /  /   /  /       _/  / /  /        /  /______  /  / \  \___            )
	$(info =/__/   /__/       /___/ /__/        /_________/ /__/   \__\____          )  
	$(info =                ___ _____      __________  __________  ________          )  
	$(info =                     /  /     /  _______/ /  _______/ /  ___   \         )
	$(info =              __ ___/  /     /  /   /  / /  /   /  / /  /   /  /         )             
	$(info =                   /  /     /  /   /  / /  /   /  / /  /___/__/          )       
	$(info =           _ _____/  /     /  /   /  / /  /   /  / /  _____/             )
	$(info =                 /  /     /  /   /  / /  /   /  / /  /                   ) 
	$(info =             ___/  /___  /__/   /  / /__/   /  / /  /                    )
	$(info =          __ __/______/ /______/__/ /______/__/ /__/                     )  

.PHONY: msg_d
msg_d : 
	$(info  =====================)
	$(info  ===Compiling Debug===)
	$(info  =====================)

.PHONY: msg_s
msg_s : 
	$(info  =====================)
	$(info  ===Compiling   Sim===)
	$(info  =====================)

.PHONY: mkdir_obj
mkdir_obj:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)/StateMachineCompact
	@mkdir -p $(OBJ_DIR)/tests


 
####
# All Build Rules are similar, except for different names and slightly different paths. 
# Instead of having just two rules (as I did before), this new system - while more complicated
# - will allow all executables to be built at the same time, without any `make clean`s.
####

#####
# build
#####
$(POD) : CXX 	= $(CXX_NORM)
$(POD) : CFLAGS = $(CFLAGS_RELEASE)
$(POD) : LD  	= $(LD_NORM)
$(POD) : build-$(POD)
build-$(POD) : $(OBJ:%.o=$(OBJ_DIR)/%-build.o)
	$(CXX) $? $(LDFLAGS) -o $(POD)

$(OBJ_DIR)/%-build.o : %.cpp
	$(LD) $? $(CFLAGS) -o $@

#####
# build-debug
#####
$(POD_D) : CXX		= $(CXX_NORM)
$(POD_D) : CFLAGS  	= $(CFLAGS_DEBUG)
$(POD_D) : LD  		= $(LD_NORM)
$(POD_D) : build-$(POD_D)
build-$(POD_D) : $(OBJ:%.o=$(OBJ_DIR)/%-build-debug.o)
	$(CXX) $? $(LDFLAGS) -o $(POD_D)

$(OBJ_DIR)/%-build-debug.o : %.cpp
	$(LD) $? $(CFLAGS) -o $@

#####
# build-test
#####
$(POD_T) : CXX 		= $(CXX_NORM)
$(POD_T) : CFLAGS  	= $(CFLAGS_SIM)
$(POD_T) : LD  		= $(LD_NORM)
$(POD_T) : build-$(POD_T)
build-$(POD_T) : $(OBJ:%.o=$(OBJ_DIR)/%-build-sim.o)
	$(CXX) $? $(LDFLAGS) -o $(POD_T)

$(OBJ_DIR)/%-build-sim.o : %.cpp
	$(LD) $? $(CFLAGS) -o $@

#####
# cross
#####
$(POD_CROSS) : CXX 	= $(CXX_BBB)
$(POD_CROSS) : CFLAGS  	= $(CFLAGS_RELEASE)
$(POD_CROSS) : LD  	= $(LD_BBB)
$(POD_CROSS) : build-$(POD_CROSS)
build-$(POD_CROSS) : $(OBJ:%.o=$(OBJ_DIR)/%-cross.o)
	$(CXX) $? $(LDFLAGS) -o $(POD_CROSS)

$(OBJ_DIR)/%-cross.o : %.cpp
	$(LD) $? $(CFLAGS) -o $@

#####
# cross-debug
#####
$(POD_CROSS_D) : CXX 	= $(CXX_BBB)
$(POD_CROSS_D) : CFLAGS = $(CFLAGS_DEBUG)
$(POD_CROSS_D) : LD     = $(LD_BBB)
$(POD_CROSS_D) : build-$(POD_CROSS_D)
build-$(POD_CROSS_D) : $(OBJ:%.o=$(OBJ_DIR)/%-cross-debug.o)
	$(CXX) $? $(LDFLAGS) -o $(POD_CROSS_D)

$(OBJ_DIR)/%-cross-debug.o : %.cpp
	$(LD) $? $(CFLAGS) -o $@

#####
# cross-test
#####
$(POD_CROSS_T) : CXX 	= $(CXX_BBB)
$(POD_CROSS_T) : CFLAGS = $(CFLAGS_SIM)
$(POD_CROSS_T) : LD  	= $(LD_BBB)
$(POD_CROSS_T) : build-$(POD_CROSS_T)
build-$(POD_CROSS_T) : $(OBJ:%.o=$(OBJ_DIR)/%-cross-sim.o)
	$(CXX) $? $(LDFLAGS) -o $(POD_CROSS_T)

$(OBJ_DIR)/%-cross-sim.o : %.cpp
	$(LD) $? $(CFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(POD) $(POD_D) $(POD_T) $(POD_CROSS) $(POD_CROSS_D) $(POD_CROSS_T)
	rm -f $(OBJ_DIR)/*.o  $(OBJ_DIR)/StateMachineCompact/*.o
	rm -f $(OBJ_DIR)/*.d  $(OBJ_DIR)/StateMachineCompact/*.d

.PHONY: clean_lib
clean_lib:
	rm -f $(LIB_DIR)/*

.PHONY: push
push: $(POD_CROSS) $(POD_CROSS_D) $(POD_CROSS_T)
	scp $(POD_CROSS)   debian@192.168.137.100:~/
	scp $(POD_CROSS_D) debian@192.168.137.100:~/
	scp $(POD_CROSS_T) debian@192.168.137.100:~/

