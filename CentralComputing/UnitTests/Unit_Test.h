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

    string name = "Define Test name";
    string cmd  = "Define Test command";

    //Needs to be virtual
    virtual ~Unit_Test();

  private:
    void display_help(Unit_Test* test_list[], int length);
};

#endif
