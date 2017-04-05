# Set up source file list and obj list
OBJ_DIR := .objs
SRC := $(shell find . -name "*.cpp")
SRC := $(patsubst ./%.cpp, %.cpp, $(SRC))
SRC := $(sort $(SRC))
OBJ := $(patsubst %.cpp, %.o, $(SRC))

# Set up compiler
CXX_NORM	:= g++
CXX_BBB 	:= arm-linux-gnueabihf-g++
WARNINGS 	:= -Wall -Wextra 

CFLAGS_DEBUG   	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP -IStateMachineCompact/ -IUnitTests/ -I. -D_GNU_SOURCE -pthread -DDEBUG 
CFLAGS_RELEASE 	:= -O2 $(WARNINGS) -g -std=c++11 -c -MMD -MP -IStateMachineCompact/ -IUnitTests/ -I. -D_GNU_SOURCE -pthread -DNDEBUG
CFlAGS_T 				:= $(CFLAGS_DEBUG) -DTEST_POD

# Set up linker
LDFLAGS := -pthread

# Define all executables
POD 				= build
POD_D 			= build-debug
POD_T 			= build-test
POD_CROSS 	= cross
POD_CROSS_D = cross-debug
POD_CROSS_T = cross-test

.PHONY: all
all : mkdir_obj msg $(POD_D)

.PHONY: msg
msg : 
	$(info Usage: make [compile type]  build  build_debug build_test  cross cross_debug cross_test)
	$(info ======= making build_debug version =======)
	echo $(OBJ)

.PHONY: mkdir_obj
mkdir_obj:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/StateMachineCompact
	@mkdir -p $(OBJ_DIR)/UnitTests

####
# All Build Rules are similar, except for different names and slightly different paths. 
# Instead of having just two rules (as I did before), this new system - while more complicated
# - will allow all executables to be built at the same time, without any `make clean`s.
####

#####
# build
#####
$(POD) : CXX 		 	= $(CXX_NORM)
$(POD) : CFLAGS  	= $(CFLAGS_RELEASE)
$(POD) : LD  			= $(LD_NORM)
$(POD) : EXENAME	= $(POD)
$(POD) : $(OBJ:%.o=$(OBJ_DIR)/%-rbuild.o)
	$(CXX) $? $(LDFLAGS) -o $@

$(OBJ_DIR)/%-rbuild.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# build-debug
#####
$(POD_D) : CXX 		 	= $(CXX_NORM)
$(POD_D) : CFLAGS  	= $(CFLAGS_DEBUG)
$(POD_D) : LD  			= $(LD_NORM)
$(POD_D) : EXENAME	= $(POD_D)
$(POD_D) : $(OBJ:%.o=$(OBJ_DIR)/%-xx.o)
	$(CXX) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%-xx.o : %.cpp
	$(CXX) $< $(CFLAGS) -o $@


#####
# build-test
#####
$(POD_T) : CXX 		 	= $(CXX_NORM)
$(POD_T) : CFLAGS  	= $(CFlAGS_T)
$(POD_T) : LD  			= $(LD_NORM)
$(POD_T) : EXENAME	= $(POD_T)
$(POD_T) : $(OBJ:%.o=$(OBJ_DIR)/%-build-test.o)
	$(CXX) $? $(LDFLAGS) -o $@

$(OBJ_DIR)/%-build-test.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# cross
#####
$(POD_CROSS) : CXX 		 	= $(CXX_BBB)
$(POD_CROSS) : CFLAGS  	= $(CFLAGS_RELEASE)
$(POD_CROSS) : LD  			= $(LD_BBB)
$(POD_CROSS) : EXENAME	= $(POD_CROSS)
$(POD_CROSS) : $(OBJ:%.o=$(OBJ_DIR)/%-cross.o)
	$(CXX) $? $(LDFLAGS) -o $@

$(OBJ_DIR)/%-cross.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# cross-debug
#####
$(POD_CROSS_D) : CXX 		 	= $(CXX_BBB)
$(POD_CROSS_D) : CFLAGS  	= $(CFLAGS_DEBUG)
$(POD_CROSS_D) : LD  			= $(LD_BBB)
$(POD_CROSS_D) : EXENAME	= $(POD_CROSS_D)
$(POD_CROSS_D) : $(OBJ:%.o=$(OBJ_DIR)/%-cross-debug.o)
	$(CXX) $? $(LDFLAGS) -o $@

$(OBJ_DIR)/%-cross-debug.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

#####
# cross-test
#####
$(POD_CROSS_T) : CXX 		 	= $(CXX_BBB)
$(POD_CROSS_T) : CFLAGS  	= $(CFlAGS_T)
$(POD_CROSS_T) : LD  			= $(LD_BBB)
$(POD_CROSS_T) : EXENAME	= $(POD_CROSS_T)
$(POD_CROSS_T) : $(OBJ:%.o=$(OBJ_DIR)/%-cross-test.o)
	$(CXX) $? $(LDFLAGS) -o $@

$(OBJ_DIR)/%-cross-test.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/StateMachineCompact/*.o $(OBJ_DIR)/UnitTests/*.o $(POD) $(POD_D) $(POD_T) $(POD_CROSS) $(POD_CROSS_D) $(POD_CROSS_T)

.PHONY: push
push: $(POD_CROSS) $(POD_CROSS_D) $(POD_CROSS_T)
	scp $(POD_CROSS) root@192.168.137.100:~/
	scp $(POD_CROSS_D) root@192.168.137.100:~/
	scp $(POD_CROSS_T) root@192.168.137.100:~/

