#include "Sensor.h"
#include <iostream>
#include <unistd.h>
#include "Pod.h"
#include "StateMachineCompact/stdafx.h"
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <stdlib.h>

#include <time.h>

using namespace std;

// declaring function, implented later
void printHelp();
void printHelpInstr();

/**
	brings out the best in all of us
*/
void memes() {
	srand(time(NULL));
	int r = rand() % 4 +1;
	cout << "MEME LORD IS HERE FOR YOU:" << endl;
	if(r == 1) {
		cout << "http://media.treehugger.com/assets/images/2013/08/elon-musk-hyperloop-good-guy-elon-musk-meme-chris-tackett.png.662x0_q70_crop-scale.png" << endl;
	} else if (r == 2) {
		cout << "http://s2.quickmeme.com/img/e6/e6b0f2395a1b7168c30c19881f51e8d20e83d08bec4d0b2a67e69a0cd18b43b1.jpg" << endl;
	} else if (r == 3) {	
		cout << "http://www.mememaker.net/static/images/memes/4284241.jpg" << endl;
	} else if (r == 4) {
		cout << "https://cdn.meme.am/cache/instances/folder695/63313695.jpg" << endl;
	}
	cout << endl;
}

/**
	checks if the current state of pod matches witht the check state command
	word: check state command to be executed
	pod: reference of pod to be checked

	return: whether the command returned true or false
*/
bool checkState(string word, Pod& pod) {
	size_t start = word.find("[");
	size_t end = word.find("]");
	string arg = word.substr(start+1,end-1);
	int state = pod.getCurrentState();

	// negates the positive claim
	bool pos = true;
	if(arg.front() == '!') {
		pos = false;
		// removes ! from argument
		arg = arg.substr(1);
	}
	
	// switch case for all the different states
	switch(state) {
		case 0:
			if(arg == "SAFE_MODE" || arg == "S_M") {
				return pos;
			}
			return !pos;
			break;
		case 1:
			if(arg == "FUNCTIONAL_TESTS" || arg == "F_T") {
				return pos;
			}
			return !pos;
			break;
		case 2:
			if(arg == "FLIGHT_ACCEL" || arg == "F_A") {
				return pos;
			}
			return !pos;
			break;
		case 3:
			if(arg == "FLIGHT_COAST" || arg == "F_C") {
				return pos;
			}
			return !pos;
			break;
		case 4:
			if(arg == "FLIGHT_BRAKE" || arg == "F_B") {
				return pos;
			}
			return !pos;
			break;
		default:
			return false;
			break;
	}
}

/**
	Processes a line of code and executes on the pod object
	word: the line of code to execute
	pod: a reference to the pod object to be used

	returns: a bool for the success of the process
		Usually true
*/
bool process( string word, Pod& pod ) {
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	// TODO add other events like sensor readings, etc;
	
	// checks for basic fsm commands
	if (word == "MOVE_FUNCTIONAL_TESTS" || word == "MV_F_T") {
		pod.move_functional_tests();
	}
	else if ( word == "MOVE_SAFE_MODE" || word == "MV_S_M") {
		pod.move_safe_mode();
	}
	else if ( word == "MOVE_FLIGHT_ACCELERATION" || word == "MV_F_A") {
		pod.move_flight_acceleration();
	}
	else if ( word == "EMERGENCY_BRAKE" || word == "E_BRAKE" ) {
		pod.emergency_brake();
	}
	else if ( word == "COAST" || word == "C" ) {
		pod.coast();
	}
	else if ( word == "BRAKE" || word == "B" ) {
		pod.brake();
	}
	// checks for utility commands
	else if ( word == "PRINT_STATE" || word == "PS" ) {
		cout << pod.get_current_state_string() << endl;
	}
	else if ( word.substr(0,1) == "[") {
		bool result = checkState(word, pod);
		if(!result) {
			cout << "warning: " << word << " failed!" << endl;
			cout << "	current state: " << pod.get_current_state_string() << endl;
		}
		return result;
	}
	// checks if command is a comment
	else if ( word.find("#") != string::npos || word == "" || word == " " ) {
		// ignored
	}
	// error catching
	else {
		// error occured in reading commands
		cout << "ERROR: instruction (" << word << ") undefined" << endl;
		cout << "       Result: Ignored" << endl;
	}
	return true;
}

/**
	Reads from a file and executes each command one by one
	fileName: file to read in
	pod: reference to pod object to be executed on
	
	prints at the end if the test was succesful
*/
void readFile( string fileName , Pod& pod) {
	bool worked = true;
	ifstream wordsFile(fileName);
	string word;
	if(wordsFile.is_open()) {
		// reads file line by line
		while (getline(wordsFile, word)) {
			//cout << "~" << word << endl;
			worked &= process(word, pod);
		}
	}

	cout << "Test was successful = " << worked << endl;
}

/**
	Reads from command line and executes user commands
	pod: reference to pod object to be executed on
*/
void manual(Pod& pod) {
	string line = "";
	// loop until user types end or quit
	while(true) {
		// prints the carrot for beauty's sake
		cout << ">";
		
		// reads from command line
		cin >> line;

		// a few special instructions that process func doesn't include
		if(line == "help" || line == "-h" ) {
			printHelp();
		}
		if(line == "help_instructions" || line == "-hi" ) {
			printHelpInstr();
		}
		// exit condition for loop
		else if(line == "end" || line == "exit" ) {
			return;
		}
		else {
			// runs through the same process func that readFile uses
			process(line, pod);
		}
	}
}

/**
	Monkey test, does random functional calls to see if pod would break
	num: the number of commands to run before terminating
	pod: reference to pod object to be executed on
*/
void random(int num, Pod& pod) {
	// sets seed for rng
	srand(time(NULL));

	cout << "random test for " << num << "cycles" << endl;
	
	for(int i = 0; i < num; i++) {
		// change the m in (rand() % m + 1) to change the maximum value
		int r = rand() % 6 +1;
		// each possible int has a correlated function
		if ( r == 1 ) {
			cout << "~move_functinal_tests"<< endl;
			pod.move_functional_tests();
		}
		else if ( r == 2 ) {
			cout << "~move_safe_mode"<< endl;
			pod.move_safe_mode();
		}
		else if ( r == 3 ) {
			cout << "~move_flight_acceleration"<< endl;
			pod.move_flight_acceleration();
		}
		else if ( r == 4 ) {
			cout << "~emergency_brake"<< endl;
			pod.emergency_brake();
		}
		else if ( r == 5 ) {
			cout << "~coast"<< endl;
			pod.coast();
		}
		else if ( r == 6 ) {
			cout << "~brake"<< endl;
			pod.brake();
		}
	}
}

/**
	A default test for the pod, for easy testing
	pod: reference to pod object to be executed on
*/
void defaultTest(Pod& pod) {
	cout << "doing default test here" << endl;
	//pod.move_functional_tests();
	cout << pod.get_current_state_string() << endl;
}
/**
	main function
	param: an array of strings to be parsed into a command";
*/
int main(int argc, char** argv) {
	Pod pod;
	// argument vector
	vector<string> args(argv, argv + argc);

	// too few arguments
	if(argc == 1) {
		cout << "Enter Arguments...type help or -h" << endl;
	}
	else if (args[1] == "default") {
		defaultTest(pod);
	}
	// complex commands
	else {
		if(args[1] == "load") {
			if(argc >= 3) {
				readFile(args[2], pod);
			}
			else if(args[1] == "load" && argc < 3) {
				cout << "Please enter a file name" << endl;
			}
		}
		else if(args[1] == "manual") {
			manual(pod);
		}
		else if(args[1] == "random") {
			if(argc >= 3) {
				random(std::stoi(args[2]), pod);
			}
			else {
				random(10,pod);
			}
		}
		// tutorial commands
		else if(args[1] == "help" || args[1] == "-h") {
			printHelp();
		}
		else if(args[1] == "help_instructions" || args[1] == "-hi" ) {
			printHelpInstr();
		}
		else if(args[1] == "memes") {
			memes();
		}
		// command failed to be recognized
		else {
			cout << "command unrecognized try again or type help" << endl;
		}
	}
		
	/*Sensor_Configuration c;
	c.type = THERMOCOUPLE;
	c.simulation = 1;
	c.count = 4;

	vector<Sensor_Configuration> configs;
	configs.push_back(c);

	Sensor_Package p(configs);
	usleep(200000);
	p.update();
	
	cout << "Mostly works!" << endl;
	*/
}

/**
	prints out all the commands that can be used
*/
void printHelp() {
	cout << "======================" << endl;
	cout << "ILLINI HYPER LOOOP SIM" << endl;
	cout << "======================" << endl;
	cout << "AUTHOR: ELON MUSK" << endl;
	cout << "CREATED: 2017" << endl;
	cout << "----------------------" << endl;
	cout << "Commands:" << endl;
	cout << "help" << endl;
	cout << "default" << endl;
	cout << "load (file_name)" << endl;
	cout << "manual" << endl;	
	cout << "...end" << endl;	
	cout << "random" << endl;
	cout << "help_instructions" << endl;
	cout << "memes" << endl;
}

/**
	prints out commands that can be used for scripting
*/
void printHelpInstr() {
	cout << "________________________________________________" << endl;
	cout << "List w/ explanations of instructions for testing" << endl;
	cout << "	Can also be called with -hi" << endl;
	cout << "each line has instructions with abrv inside ()" << endl;
	cout << "tests can be done with full instructs and abrv" << endl;
	cout << "some functions like help can still be called" << endl;	
	cout << "------------------------------------------------" << endl;
	cout << "MOVE_FUNCTIONAL_TESTS (MV_F_T)" << endl;
	cout << "MOVE_SAFE_MODE (MV_S_M)" << endl;
	cout << "MOVE_FLIGHT_ACCELERATION (MV_F_A)" << endl;
	cout << "EMERGENCY_BRAKE (E_BRAKE)" << endl;
	cout << "COAST (c)" << endl;
	cout << "BRAKE (b)" << endl;
	cout << "---utility---" <<endl;
	cout << "PRINT_STATE (ps)" << endl;
	cout << "---checker---" <<endl;
	cout << "You can check for being in a state with []" << endl;
	cout << "ex: [STATE_NAME]" << endl;
	cout << "You can also check for not being in a state" << endl;
	cout << "ex: [!STATE_NAME]" << endl;
	cout << "Here's a list of state names:" << endl;
	cout << "	safe_mode (s_m)" << endl;
	cout << "	functional_tests (f_t)" << endl;
	cout << "	flight_accel (f_a)" << endl;
	cout << "	flight_coast (f_c)" << endl;
	cout << "	flight_brake (f_b)" << endl;
	cout << endl;
	cout << "Note: commenting out lines can be done with #" << endl;
	cout << "Note: don't indent or start lines with spaces" << endl;
}
