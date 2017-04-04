#ifndef _UNIT_TEST_H
#define _UNIT_TEST_H

#include <string>
#include <vector>
using namespace std;

class Unit_Test {

  public:
    /**
    * Runs any tests defined in this function
    * Since this is a pure virtual function, subclasses are to extend it
    *
    * Correct way to call this parent function is:
    * ./exe command [optional args depending on command]
    *	@param argc length of argv 
    * @param argv array of input commands 
    * The command will be parsed, and the correct Unit_Test subclass will be run
    **/
    virtual int test(int argc, char** argv);

    /**
    * name and cmd are used when printing out help options in the testing suite,
    * in order to easily identify which test does what. 
    *
    * cmd is tested against the input command. If they are equal, that unit test
    * is run.
    *
    * These strings are defined here such that all subclasses have access to these
    * variables. Each subclass should define the name and cmd in their constructor
    *
    * Please refer to Spi_Test.cpp, and the README file inside the UnitTest/ directory
    *
    **/
    string name = "Define Test name";
    string cmd  = "Define Test command";

    //Needs to be virtual
    virtual ~Unit_Test();

  private:
    void display_help(Unit_Test* test_list[], int length);
};

#endif
