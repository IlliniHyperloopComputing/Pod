#ifndef _SENSOR_TEST_H
#define _SENSOR_TEST_H

#include "Sensor.h"
#include "Pod.h"
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Sensor_Test {

  public:
    /**
    *	Acts as main()
    *	@param: an array of strings to be parsed into a command
    **/
    static int start_test(int argc, char** argv);

    /**
    * Edgy Memes
    *	brings out the best in all of us
    **/
    static void memes();

  private:

    /**
    *	Checks if the current state of pod matches with the check state command
    *	@param word  check state command to be executed
    *	@param pod  reference of pod to be checked
    *
    *	@param return  If the current state is the same as in command
    **/
    static bool assert_state_equals(string word, Pod& pod);

    /**
    *	Checks if the current state of pod matches with the check state command
    *	@param word  check state command to be executed
    *	@param pod  reference of pod to be checked
    *
    *	@param return  If the current state is not the same as in command
    **/
    static bool assert_state_not_equals(string command, Pod& pod);

    /**
    *	Processes a line of code and executes on the pod object
    *	@param word  The line of code to execute
    *	@param pod  A reference to the pod object to be used
    *
    *	@return A bool for the success of the process
    *		Usually true
    **/
    static bool process(string word, Pod& pod);

    /**
    *	Reads from a file and executes each command one by one
    *	@param fileName file to read in
    *	@param pod reference to pod object to be executed on
    *	
    *	prints at the end if the test was succesful
    **/
    static void read_file(string fileName, Pod& pod);

    /**
    *	Reads from command line and executes user commands
    *	@param pod reference to pod object to be executed on
    **/
    static void manual(Pod& pod);

    /**
    *	Monkey test, does random functional calls to see if pod would break
    *	@param num  the number of commands to run before terminating
    *	@param pod  reference to pod object to be executed on
    **/
    static void random(int num, Pod& pod);

    /**
    *	A default test for the pod, for easy testing
    *	@param pod  A reference to the pod object 
    **/
    static void default_test(Pod& pod);

    /**
    *	Prints out all the commands that can be used
    **/
    static void print_help();

    /**
    * Prints out scripting guide 
    **/
    static void print_help_instr();

};

#endif
