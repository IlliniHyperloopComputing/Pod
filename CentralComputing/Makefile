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


INCLUDE_DIRS	:= -IStateMachineCompact -I${GTEST_DIR}/include -I. -Iscenarios
CFLAGS_RELEASE 	:= -O2 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DNDEBUG
CFLAGS_DEBUG   	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DDEBUG 
CFLAGS_SIM 	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP $(INCLUDE_DIRS) -D_GNU_SOURCE -pthread -DDEBUG -DSIM 
CFLAGS_BBB      := -DBBB
CFLAGS_NORM     := 
#Set up linker
LDFLAGS := -pthread -lm  #-static-libgcc -static-libstdc++ 
# The 'static' flags might be necessary if the cross compiler and BBB have different versions of the libgcc and libstdc++ libraries. 
# This is why these flags were intially included, but now the BBBs are running newer software such that these are no longer needed.
# However, the 'static' flags will likely not work with any of the Sanitizers
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
all : msg mkdir_obj ${LIB_DIR}/gtest-all.o ${LIB_DIR}/gtest-all-arm.o msg_d $(POD_D)-tsan msg_s $(POD_T)-tsan
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

.PHONY: lint
lint:
	python cpplint/cpplint/cpplint.py --filter=-legal/copyright,-build/include,-whitespace/end_of_line,-readability/todo --linelength=120 --quiet --extensions=cpp,h,hpp --root=. *.cpp *.hpp *.h

.PHONY: mkdir_obj
mkdir_obj:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)/StateMachineCompact
	@mkdir -p $(OBJ_DIR)/tests
	@mkdir -p $(OBJ_DIR)/scenarios

# Compile libgtest
${LIB_DIR}/gtest-all.o : ${GTEST_DIR}/src/gtest-all.cc 
	$(CXX_NORM) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o ${LIB_DIR}/gtest-all.o
	ar -rv ${LIB_DIR}/libgtest.a ${LIB_DIR}/gtest-all.o

ifdef ARM_COMPILER_EXISTS
${LIB_DIR}/gtest-all-arm.o : ${GTEST_DIR}/src/gtest-all.cc 
	$(CXX_BBB) -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc -o ${LIB_DIR}/gtest-all-arm.o 
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

.PHONY: msg_p
msg_p : 
	$(info  =====================)
	$(info  ===Compiling  Prod===)
	$(info  =====================)

 
####
# All Build Rules are similar, except for different names and slightly different paths. 
# Instead of having just two rules (as I did before), this new system - while more complicated
# - will allow all executables to be built at the same time, without any `make clean`s.
####

#####
# build
#####
$(POD)-tsan : LD_NORM := $(LD_NORM) -fsanitize=thread
$(POD)-tsan : CXX_NORM := $(CXX_NORM) -fsanitize=thread -fPIE -pie
$(POD)-tsan : $(POD)  #JUMP TO POD to continue compiling

$(POD)-asan : LD_NORM := $(LD_NORM) -fsanitize=address
$(POD)-asan : CXX_NORM := $(CXX_NORM) -fsanitize=address 
$(POD)-asan : $(POD)  #JUMP TO POD to continue compiling

$(POD) : CXX 	= $(CXX_NORM)
$(POD) : CFLAGS = $(CFLAGS_RELEASE) $(CFLAGS_NORM)
$(POD) : LD  	= $(LD_NORM)
$(POD) : mkdir_obj build-$(POD)
build-$(POD) : $(OBJ:%.o=$(OBJ_DIR)/%-build.o)
	$(LD) $? $(LDFLAGS) $(LD_TEST_NORM)  -o $(POD)

$(OBJ_DIR)/%-build.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# build-debug
#####
$(POD_D)-tsan : LD_NORM := $(LD_NORM) -fsanitize=thread
$(POD_D)-tsan : CXX_NORM := $(CXX_NORM) -fsanitize=thread -fPIE -pie
$(POD_D)-tsan : $(POD_D)  #JUMP TO POD to continue compiling

$(POD_D)-asan : LD_NORM := $(LD_NORM) -fsanitize=address
$(POD_D)-asan : CXX_NORM := $(CXX_NORM) -fsanitize=address 
$(POD_D)-asan : $(POD_D)  #JUMP TO POD to continue compiling

$(POD_D) : CXX		= $(CXX_NORM)
$(POD_D) : CFLAGS  	= $(CFLAGS_DEBUG) $(CFLAGS_NORM)
$(POD_D) : LD  		= $(LD_NORM)
$(POD_D) : mkdir_obj build-$(POD_D)
build-$(POD_D) : $(OBJ:%.o=$(OBJ_DIR)/%-build-debug.o)
	$(LD) $? $(LDFLAGS) $(LD_TEST_NORM) -o $(POD_D)

$(OBJ_DIR)/%-build-debug.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# build-test
#####
$(POD_T)-tsan : LD_NORM := $(LD_NORM) -fsanitize=thread
$(POD_T)-tsan : CXX_NORM := $(CXX_NORM) -fsanitize=thread -fPIE -pie
$(POD_T)-tsan : $(POD_T)  #JUMP TO POD to continue compiling

$(POD_T)-asan : LD_NORM := $(LD_NORM) -fsanitize=address
$(POD_T)-asan : CXX_NORM := $(CXX_NORM) -fsanitize=address 
$(POD_T)-asan : $(POD_T)  #JUMP TO POD to continue compiling

$(POD_T) : CXX 		= $(CXX_NORM)
$(POD_T) : CFLAGS  	= $(CFLAGS_SIM) $(CFLAGS_NORM)
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
$(POD_CROSS) : CFLAGS  	= $(CFLAGS_RELEASE) $(CFLAGS_BBB)
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
$(POD_CROSS_D) : CFLAGS = $(CFLAGS_DEBUG) $(CFLAGS_BBB)
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
$(POD_CROSS_T) : CFLAGS = $(CFLAGS_SIM) $(CFLAGS_BBB)
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

BBB := bbb1
ADDR := 128.174.163.125

.PHONY: push
push: 
	$(info  ==============================================)
	$(info  Ways to push to BBB:)
	${info  (1) `make push_all`    for all of the following}
	${info  (2) `make push_sim`    for sim build}
	${info  (3) `make push_debug`  for debug build}
	${info  (4) `make push_prod`   for optimized build}
	${info  (5) `make push_config` for config file}
	$(info  ==============================================)

.PHONY: push_all
push_all: msg push_sim push_debug push_prod 

.PHONY: push_sim
push_sim: msg_s mkdir_obj $(POD_CROSS_T) 
	scp $(POD_CROSS_T) $(BBB)@$(ADDR):~/

.PHONY: push_debug
push_debug: msg_d mkdir_obj $(POD_CROSS_D) 
	scp $(POD_CROSS_D) $(BBB)@$(ADDR):~/

.PHONY: push_prod
push_prod: msg_p mkdir_obj $(POD_CROSS)
	scp $(POD_CROSS) $(BBB)@$(ADDR):~/

.PHONY: push_config
push_config: 
	scp defaultConfig.txt $(BBB)@$(ADDR):~/

