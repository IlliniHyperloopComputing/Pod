# Set up source file list and obj list
OBJ_DIR := .objs
SRC := $(shell find . -name "*.cpp")
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
POD_D 			= build_debug
POD_T 			= build_test
POD_CROSS 	= cross
POD_CROSS_D = cross_debug
POD_CROSS_T = cross_test

all : mkdir_obj msg $(POD_D)

.PHONY: msg
msg : 
	$(info Usage: make [compile type]  build  build_debug build_test  cross cross_debug cross_test)
	$(info ======= making build_debug version =======)

.PHONY: mkdir_obj
mkdir_obj:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/StateMachineCompact
	@mkdir -p $(OBJ_DIR)/UnitTests

#Setup specific build rules
#This allows for one set of build rules

$(POD) : CXX 		 	= $(CXX_NORM)
$(POD) : CFLAGS  	= $(CFLAGS_RELEASE)
$(POD) : LD  			= $(LD_NORM)
$(POD) : EXENAME	= $(POD)
$(POD) : EXE

$(POD_D) : CXX 		 	= $(CXX_NORM)
$(POD_D) : CFLAGS  	= $(CFLAGS_DEBUG)
$(POD_D) : LD  			= $(LD_NORM)
$(POD_D) : EXENAME	= $(POD_D)
$(POD_D) : EXE

$(POD_T) : CXX 		 	= $(CXX_NORM)
$(POD_T) : CFLAGS  	= $(CFlAGS_T)
$(POD_T) : LD  			= $(LD_NORM)
$(POD_T) : EXENAME	= $(POD_T)
$(POD_T) : EXE


$(POD_CROSS) : CXX 		 	= $(CXX_BBB)
$(POD_CROSS) : CFLAGS  	= $(CFLAGS_RELEASE)
$(POD_CROSS) : LD  			= $(LD_BBB)
$(POD_CROSS) : EXENAME	= $(POD_CROSS)
$(POD_CROSS) : EXE

$(POD_CROSS_D) : CXX 		 	= $(CXX_BBB)
$(POD_CROSS_D) : CFLAGS  	= $(CFLAGS_DEBUG)
$(POD_CROSS_D) : LD  			= $(LD_BBB)
$(POD_CROSS_D) : EXENAME	= $(POD_CROSS_D)
$(POD_CROSS_D) : EXE

$(POD_CROSS_T) : CXX 		 	= $(CXX_BBB)
$(POD_CROSS_T) : CFLAGS  	= $(CFlAGS_T)
$(POD_CROSS_T) : LD  			= $(LD_BBB)
$(POD_CROSS_T) : EXENAME	= $(POD_CROSS_T)
$(POD_CROSS_T) : EXE

#include dependencies
#-include $(OBJ_DIR)/*.d

#
# LINK LINK LINK LINK
#
EXE : $(OBJ:%.o=$(OBJ_DIR)/%.o)
	$(CXX) $? $(LDFLAGS) -o $(EXENAME)

#
# COMPILE COMPILE COMPILE
#
$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/StateMachineCompact/*.o $(POD) $(POD_D) $(POD_T) $(POD_CROSS) $(POD_CROSS_D) $(POD_CROSS_T)

push:
	scp $(POD_CROSS_D) root@192.168.137.100:~/

