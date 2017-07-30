//Create header guards
#ifndef _STATEMACHINE_TEST_H
#define _STATEMACHINE_TEST_H

//Need to include Unit_Test in order to extend it
#include "Unit_Test.h"

//Any other dependencies 
#include "Pod_State.h"
#include "Sensor.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>

class StateMachine_Test : public Unit_Test {
  public: 
    /**
    * Implementing test() of virtual function
    **/
    int test(int argc, char** argv);

    /**
    * Use the constructor to define name and cmd fields
    **/
    StateMachine_Test();

  //Can define Anything in private section
  private:
    /**
    *	Checks if the current state of pod matches with the check state command
    *	@param word  check state command to be executed
    *	@param pod  reference of pod to be checked
    *
    *	@param return  If the current state is the same as in command
    **/
    static bool assert_state_equals(string word, Pod_State& pod);

    /**
    *	Checks if the current state of pod matches with the check state command
    *	@param word  check state command to be executed
    *	@param pod  reference of pod to be checked
    *
    *	@param return  If the current state is not the same as in command
    **/
    static bool assert_state_not_equals(string command, Pod_State& pod);

    /**
    *	Processes a line of code and executes on the pod object
    *	@param word  The line of code to execute
    *	@param pod  A reference to the pod object to be used
    *
    *	@return A bool for the success of the process
    *		Usually true
    **/
    static bool process(string word, Pod_State& pod);

    /**
    *	Reads from a file and executes each command one by one
    *	@param fileName file to read in
    *	@param pod reference to pod object to be executed on
    *	
    *	prints at the end if the test was succesful
    **/
    static void read_file(string fileName, Pod_State& pod);

    /**
    *	Reads from command line and executes user commands
    *	@param pod reference to pod object to be executed on
    **/
    static void manual(Pod_State& pod);

    /**
    *	Monkey test, does random functional calls to see if pod would break
    *	@param num  the number of commands to run before terminating
    *	@param pod  reference to pod object to be executed on
    **/
    static void random(int num, Pod_State& pod);

    /**
    *	A default test for the pod, for easy testing
    *	@param pod  A reference to the pod object 
    **/
    static void default_test(Pod_State& pod);

    /**
    *	Prints out all the commands that can be used
    **/
    static void print_help();

    /**
    * Prints out scripting guide 
    **/
    static void print_script_instr();

    /**
    * Edgy Memes
    *	brings out the best in all of us
    **/
    static void memes();
};

#endif
