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
CXX_NORM	:= g++ -fpermissive
CXX_BBB 	:= arm-linux-gnueabihf-g++
WARNINGS 	:= -Wall -Wextra -pedantic -Wdouble-promotion -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wno-unused

INCLUDE_DIRS	:= -IStateMachineCompact -I${GTEST_DIR}/include -I.
CFLAGS_RELEASE 	:= -O2 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DNDEBUG
CFLAGS_DEBUG   	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DDEBUG 
CFLAGS_SIM 	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DDEBUG -DSIM

#Set up linker
LDFLAGS := -pthread -lm -static-libgcc -static-libstdc++ 
LD_TEST_NORM := ${LIB_DIR}/libgtest.a
LD_TEST_ARM  := ${LIB_DIR}/libgtest-arm.a
LD_NORM := $(CXX_NORM)
LD_BBB  := $(CXX_BBB)

# Define all executables
POD = build
POD_D = dbuild
POD_T = sbuild
POD_CROSS = cross
POD_CROSS_D = dcross
POD_CROSS_T = scross

ARM_COMPILER_EXISTS := $(shell command -v $(CXX_BBB) 2> /dev/null)


.PHONY: all
all : msg mkdir_obj ${LIB_DIR}/gtest-all.o ${LIB_DIR}/gtest-all-arm.o msg_d $(POD_D) msg_s $(POD_T)
	$(info  =   ___ _   _  ___ ___ ___  ___ ___  )
	$(info	=  / __| | | |/ __/ __/ _ \/ __/ __| )
	${info	=  \__ \ |_| | (_| (_|  __/\__ \__ \ }
	$(info	=  |___/\__,_|\___\___\___||___/___/ )	

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

.PHONY: mkdir_obj
mkdir_obj:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)/StateMachineCompact
	@mkdir -p $(OBJ_DIR)/tests

# Compile libgtest
${LIB_DIR}/gtest-all.o : ${GTEST_DIR}/src/gtest-all.cc 
	g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o ${LIB_DIR}/gtest-all.o
	ar -rv ${LIB_DIR}/libgtest.a ${LIB_DIR}/gtest-all.o

ifdef ARM_COMPILER_EXISTS
${LIB_DIR}/gtest-all-arm.o : ${GTEST_DIR}/src/gtest-all.cc 
	arm-linux-gnueabihf-g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o ${LIB_DIR}/gtest-all-arm.o
	ar -rv ${LIB_DIR}/libgtest-arm.a ${LIB_DIR}/gtest-all-arm.o
endif

ifndef ARM_COMPILER_EXISTS
${LIB_DIR}/gtest-all-arm.o : ${GTEST_DIR}/src/gtest-all.cc 
	$(info = Cant compile for ARM. install arm-linux-gnueabihf-g++)
endif

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
$(POD) : mkdir_obj build-$(POD)
build-$(POD) : $(OBJ:%.o=$(OBJ_DIR)/%-build.o)
	$(LD) $? $(LDFLAGS) $(LD_TEST_NORM)  -o $(POD)

$(OBJ_DIR)/%-build.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# build-debug
#####
$(POD_D) : CXX		= $(CXX_NORM)
$(POD_D) : CFLAGS  	= $(CFLAGS_DEBUG)
$(POD_D) : LD  		= $(LD_NORM)
$(POD_D) : mkdir_obj build-$(POD_D)
build-$(POD_D) : $(OBJ:%.o=$(OBJ_DIR)/%-build-debug.o)
	$(LD) $? $(LDFLAGS) $(LD_TEST_NORM) -o $(POD_D)

$(OBJ_DIR)/%-build-debug.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# build-test
#####
$(POD_T) : CXX 		= $(CXX_NORM)
$(POD_T) : CFLAGS  	= $(CFLAGS_SIM)
$(POD_T) : LD  		= $(LD_NORM)
$(POD_T) : mkdir_obj build-$(POD_T)

build-$(POD_T) : $(OBJ:%.o=$(OBJ_DIR)/%-build-sim.o)
	$(LD) $? $(LDFLAGS) $(LD_TEST_NORM) -o $(POD_T)

$(OBJ_DIR)/%-build-sim.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# cross
#####
$(POD_CROSS) : CXX 	= $(CXX_BBB)
$(POD_CROSS) : CFLAGS  	= $(CFLAGS_RELEASE)
$(POD_CROSS) : LD  	= $(LD_BBB)
$(POD_CROSS) : mkdir_obj build-$(POD_CROSS)
build-$(POD_CROSS) : $(OBJ:%.o=$(OBJ_DIR)/%-cross.o)
	$(LD) $? $(LDFLAGS) $(LD_TEST_ARM) -o $(POD_CROSS)

$(OBJ_DIR)/%-cross.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# cross-debug
#####
$(POD_CROSS_D) : CXX 	= $(CXX_BBB)
$(POD_CROSS_D) : CFLAGS = $(CFLAGS_DEBUG)
$(POD_CROSS_D) : LD     = $(LD_BBB)
$(POD_CROSS_D) : mkdir_obj build-$(POD_CROSS_D)
build-$(POD_CROSS_D) : $(OBJ:%.o=$(OBJ_DIR)/%-cross-debug.o)
	$(CXX) $? $(LDFLAGS) $(LD_TEST_ARM) -o $(POD_CROSS_D)

$(OBJ_DIR)/%-cross-debug.o : %.cpp
	$(LD) $? $(CFLAGS) -o $@

#####
# cross-test
#####
$(POD_CROSS_T) : CXX 	= $(CXX_BBB)
$(POD_CROSS_T) : CFLAGS = $(CFLAGS_SIM)
$(POD_CROSS_T) : LD  	= $(LD_BBB)
$(POD_CROSS_T) : mkdir_obj build-$(POD_CROSS_T)
build-$(POD_CROSS_T) : $(OBJ:%.o=$(OBJ_DIR)/%-cross-sim.o)
	$(LD) $? $(LDFLAGS) $(LD_TEST_ARM) -o $(POD_CROSS_T)

$(OBJ_DIR)/%-cross-sim.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(POD) $(POD_D) $(POD_T) $(POD_CROSS) $(POD_CROSS_D) $(POD_CROSS_T)
	rm -rf $(OBJ_DIR)/  

.PHONY: clean_lib
clean_lib:
	rm -f $(LIB_DIR)/*

.PHONY: push
push: msg mkdir_obj $(POD_CROSS_T) #########$(POD_CROSS) $(POD_CROSS_D) 
	# scp $(POD_CROSS)   debian@192.168.137.100:~/
	# scp $(POD_CROSS_D) debian@192.168.137.100:~/
	scp $(POD_CROSS_T) debian@128.174.163.125:~/

