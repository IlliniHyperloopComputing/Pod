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

void memes() {
	cout << "http://media.treehugger.com/assets/images/2013/08/elon-musk-hyperloop-good-guy-elon-musk-meme-chris-tackett.png.662x0_q70_crop-scale.png" << endl;
}

void process( string word, Pod& pod , bool isFile) {
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	// TODO add method to check if in right state
	// TODO add other events like sensor readings, etc;
	
	// only print if the line being process came from a file, not manual
	if(isFile) {
		cout << "~" << word << endl;
	}

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
	else if ( word.find("#") != string::npos || word == "" || word == " " ) {
		// ignored
	}
	else {
		// error occured in reading commands
		cout << "ERROR: instruction (" << word << ") undefined" << endl;
		cout << "       Result: Ignored" << endl;
	}
}

void readFile( string fileName , Pod& pod) {
	ifstream wordsFile(fileName);
	string word;
	if(wordsFile.is_open()) {
		// reads file line by line
		while (getline(wordsFile, word)) {
			process(word, pod, true);
		}
	}
}

void manual(Pod& pod) {
	string line = "";
	while(line != "end" || line != "e") {
		cout << ">";
		
		cin >> line;
		if(line == "help" || line == "-h" ) {
			printHelp();
		}
		if(line == "help_instructions" || line == "-hi" ) {
			printHelpInstr();
		}
		else if(line == "end" || line == "exit" || line == "e") {
			return;
		}
		else {
			process(line, pod, false);
		}
	}
}

// monkey test
void random(int num, Pod& pod) {
	srand(time(NULL));
	cout << "random test for " << num << "cycles" << endl;
	for(int i = 0; i < num; i++) {
		int r = rand() % 6 +1;
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

void defaultTest(Pod& pod) {
	cout << "doing default test here" << endl;
}

int main(int argc, char** argv) {
	Pod pod;
	// argument vector
	vector<string> args(argv, argv + argc);
	if(argc == 1) {
		cout << "Enter Arguments...type help or -h" << endl;
	}
	else if (args[1] == "default") {
		defaultTest(pod);
	}
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
		else if(args[1] == "help" || args[1] == "-h") {
			printHelp();
		}
		else if(args[1] == "help_instructions" || args[1] == "-hi" ) {
			printHelpInstr();
		}
		else if(args[1] == "memes") {
			memes();
		}
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

void printHelpInstr() {
	cout << "________________________________________________" << endl;
	cout << "List w/ explanations of instructions for testing" << endl;
	cout << "	Can also be called with -hi" << endl;
	cout << "each line has instructions with abrv inside ()" << endl;
	cout << "tests can be done with either full instructions or abrv" << endl;
	cout << "------------------------------------------------" << endl;
	cout << endl;
	cout << "MOVE_FUNCTIONAL_TESTS (MV_F_T)" << endl;
	cout << "MOVE_SAFE_MODE (MV_S_M)" << endl;
	cout << "MOVE_FLIGHT_ACCELERATION (MV_F_A)" << endl;
	cout << "EMERGENCY_BRAKE (E_BRAKE)" << endl;
	cout << "COAST (c)" << endl;
	cout << "BRAKE (b)" << endl;
	cout << endl;
	cout << "Note: commenting out lines can be done with #" << endl;
	cout << "Note: don't indent or start lines with spaces" << endl;
}
