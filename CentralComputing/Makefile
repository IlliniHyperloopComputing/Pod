# Set up source file list and obj list
OBJ_DIR := .objs
SRC := $(shell find . -name "*.cpp")
OBJ := $(patsubst %.cpp, %.o, $(SRC))

# Set up compiler
CXX_NORM	:= g++
CXX_BBB 	:= arm-linux-gnueabihf-g++
WARNINGS 	:= -Wall -Wextra 
CFLAGS_DEBUG   	:= -O0 $(WARNINGS) -g -std=c++11 -c -MMD -MP -D_GNU_SOURCE -pthread -DDEBUG 
CFLAGS_RELEASE 	:= -O2 $(WARNINGS) -g -std=c++11 -c -MMD -MP -D_GNU_SOURCE -pthread 

# Set up linker
LDFLAGS := -pthread

# Define all executables
POD 				= build
POD_D 			= build_test
POD_CROSS 	= cross
POD_CROSS_D = cross_test

all : 
	$(error Usage: make [compile type]  build  build_test  cross  cross_test)


mkdir_obj:
	@mkdir -p $(OBJ_DIR)

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

$(POD_CROSS) : CXX 		 	= $(CXX_CROSS)
$(POD_CROSS) : CFLAGS  	= $(CFLAGS_RELEASE)
$(POD_CROSS) : LD  			= $(LD_BBB)
$(POD_CROSS) : EXENAME	= $(POD_CROSS)

$(POD_CROSS_D) : CXX 		 	= $(CXX_CROSS)
$(POD_CROSS_D) : CFLAGS  	= $(CFLAGS_DEBUG)
$(POD_CROSS_D) : LD  			= $(LD_BBB)
$(POD_CROSS_D) : EXENAME	= $(POD_CROSS_D)

#include dependencies
-include $(OBJ_DIR)/*-$(EXENAME).d

#
# LINK LINK LINK LINK
#
EXE : mkdir_obj $(OBJ:%.o=$(OBJ_DIR)/%-$(EXENAME).o)
	$(CXX) $? $(LDFLAGS) -o $(EXENAME)

#
# COMPILE COMPILE COMPILE
#
$(OBJ_DIR)/%-$(EXENAME).o : %.cpp
	$(CXX) $? $(CFLAGS) -o $@

clean:
	rm -f $(OBJ_DIR)/*.o

