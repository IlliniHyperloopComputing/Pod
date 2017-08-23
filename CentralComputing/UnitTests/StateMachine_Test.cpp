#include "StateMachine_Test.h"

//Include any other dependencies 

StateMachine_Test::StateMachine_Test(){
  name = "State Machine Test";
  cmd  = "smt";
}

int StateMachine_Test::test(int argc, char** argv) {
  SafeQueue<Xmega_Command_t> * command_queue;
	command_queue = new SafeQueue<Xmega_Command_t>();
  Sensor_Package * sensors;
	auto configs = parse_input(argc, argv);
	sensors = new Sensor_Package(std::get<1>(configs), std::get<0>(configs));
	Pod_State pod(command_queue,sensors);
	// argument vector
	vector<string> args(argv, argv + argc);

	// too few arguments
	if(argc == 1) {
		cout << "Enter Arguments...type help or -h" << endl;
	}
	else if (args[1] == "default") {
		default_test(pod);
	}
	// complex commands
	else {
		if(args[1] == "load") {
			if(argc >= 3) {
				read_file(args[2], pod);
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
			print_help();
		}
		else if(args[1] == "script_instructions" || args[1] == "-si" ) {
			print_script_instr();
		}
		else if(args[1] == "memes") {
			memes();
		}
		// command failed to be recognized
		else {
			cout << "command unrecognized try again or type help" << endl;
		}
	}
		
  return 0;
	//usleep(200000);
}

/**
	
*/
void wait(string command, Pod_State& pod) {
	//istringstream buf(command);
	//istream_iterator<string> beg(buf), end;
	//
	//vector<string> tokens(beg, end);
	//for(auto& s:tokens) {
	//	//cout << s << endl;
	//}
}
/**
	checks if the current state of pod matches witht the check state command
	word: check state command to be executed
	pod: reference of pod to be checked
	*/


bool StateMachine_Test::assert_state_equals(string word, Pod_State& pod) {
	size_t start = word.find("[");
	size_t end = word.find("]");
	string arg = word.substr(start+1,end-1);
	int state = pod.get_current_state();
	
	// switch case for all the different states
	switch(state) {
		case Pod_State::ST_SAFE_MODE:
			return (arg == "SAFE_MODE" || arg == "S_M");
			break;
		case Pod_State::ST_FUNCTIONAL_TEST:
			return (arg == "FUNCTIONAL_TESTS" || arg == "F_T");
			break;
		case Pod_State::ST_FLIGHT_ACCEL:
			return (arg == "FLIGHT_ACCEL" || arg == "F_A");
			break;
		case Pod_State::ST_FLIGHT_COAST:
			return (arg == "FLIGHT_COAST" || arg == "F_C");
			break;
		case Pod_State::ST_FLIGHT_BRAKE:
			return (arg == "FLIGHT_BRAKE" || arg == "F_B");
			break;
		default:
			return false;
			break;
	}
}

bool StateMachine_Test::assert_state_not_equals(string command, Pod_State& pod) {
	// remove the ! from the command
	command = "[" + command.substr(1);
	
	return !assert_state_equals(command, pod);
}

bool StateMachine_Test::process(string word, Pod_State& pod) {
	std::transform(word.begin(), word.end(), word.begin(), ::toupper);
	// TODO add other events like sensor readings, etc;
	
	// checks for basic fsm commands
	if (word == "MOVE_FUNCTIONAL_TESTS" || word == "MV_F_T") {
		pod.move_functional_tests();
	}
	else if ( word == "MOVE_SAFE_MODE" || word == "MV_S_M") {
		pod.move_safe_mode();
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
		bool result = false;
		if(word.substr(1,2) == "!") {	
			result = assert_state_not_equals(word, pod);
		}
		else {
			result = assert_state_equals(word, pod);
		}	
		// prints out if the assert fails, ignores otherwise
		if(!result) {
			cout << "warning: " << word << " failed!" << endl;
			cout << "	current state: " << pod.get_current_state_string() << endl;
		}
		// for the read_file tests, all the tests are & together
		return result;
	}
	// sets a timer to wait
	else if ( word.substr(0,4) == "WAIT" || word.substr(0,1) == "W" ) {
		wait(word, pod);
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

void StateMachine_Test::read_file(string fileName, Pod_State& pod) {
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

void StateMachine_Test::manual(Pod_State& pod) {
	string line = "";
	
	// loop until user types end or quit
	while(true) {
		// prints the carrot for beauty's sake
		cout << ">";
		
		// reads from command line
		cin >> line;

		// a few special instructions that process func doesn't include
		if(line == "help" || line == "-h" ) {
			print_help();
		}
		if(line == "script_instructions" || line == "-si" ) {
			print_script_instr();
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

void StateMachine_Test::random(int num, Pod_State& pod) {
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

void StateMachine_Test::default_test(Pod_State& pod) {
	cout << "doing default test here" << endl;
	//pod.move_functional_tests();
	cout << pod.get_current_state_string() << endl;
}


void StateMachine_Test::print_help() {
	cout << "==============================" << endl;
	cout << "STATE MACHINE TESTING SUITE " << endl;
	cout << "==============================" << endl;
	cout << "Commands:" << endl;
	cout << "\thelp, -h" << endl;
	cout << "\tdefault" << endl;
	cout << "\tload (file_name)" << endl;
	cout << "\tmanual" << endl;	
	cout << "\trandom" << endl;
	cout << "\tscript_instructions, -si" << endl;
	cout << "\tmemes" << endl;
}

void StateMachine_Test::print_script_instr() {
	cout << "____________________________________________________" << endl;
	cout << "Scripting instructions for testing" << endl;
	cout << "Each line has instructions with abrv inside ()" << endl;
	cout << "Mix-and-match of full and abrv instructions allowed" << endl;
	cout << "some functions like help can still be called" << endl;	
	cout << "----------------------------------------------------" << endl;
	cout << "MOVE_FUNCTIONAL_TESTS (MV_F_T)" << endl;
	cout << "MOVE_SAFE_MODE (MV_S_M)" << endl;
	cout << "MOVE_FLIGHT_ACCELERATION (MV_F_A)" << endl;
	cout << "EMERGENCY_BRAKE (E_BRAKE)" << endl;
	cout << "COAST (c)" << endl;
	cout << "BRAKE (b)" << endl;
	cout << "---utility---" <<endl;
	cout << "PRINT_STATE (ps)" << endl;
	cout << "WAIT (W) <number> <unit>" << endl;
	cout << "	seconds (s)" << endl;
	cout << "	minutes (m)" << endl;
	cout << "---checker---" <<endl;
	cout << "You can assert the current state with []" << endl;
	cout << "ex: [STATE_NAME]" << endl;
	cout << "ex: [!STATE_NAME]" << endl;
	cout << "Here's a list of state names:" << endl;
	cout << "	safe_mode (s_m)" << endl;
	cout << "	functional_tests (f_t)" << endl;
	cout << "	flight_accel (f_a)" << endl;
	cout << "	flight_coast (f_c)" << endl;
	cout << "	flight_brake (f_b)" << endl;
	cout << endl;
	cout << "Note: Comment out lines with #" << endl;
	cout << "Note: Don't indent or start lines with spaces" << endl;
}

void StateMachine_Test::memes() {
	srand(time(NULL));
	int r = rand() % 5 +1;

	cout << "MEME LORD IS HERE FOR YOU:" << endl;
	if(r == 1) {
		cout << "http://media.treehugger.com/assets/images/2013/08/elon-musk-hyperloop-good-guy-elon-musk-meme-chris-tackett.png.662x0_q70_crop-scale.png" << endl;
	} else if (r == 2) {
		cout << "http://s2.quickmeme.com/img/e6/e6b0f2395a1b7168c30c19881f51e8d20e83d08bec4d0b2a67e69a0cd18b43b1.jpg" << endl;
	} else if (r == 3) {	
		cout << "http://www.mememaker.net/static/images/memes/4284241.jpg" << endl;
	} else if (r == 4) {
		cout << "https://cdn.meme.am/cache/instances/folder695/63313695.jpg" << endl;
	} else if (r == 5) {
		cout << "https://s-media-cache-ak0.pinimg.com/736x/92/ab/3a/92ab3abea77d3f6a397fe8545993713e.jpg" << endl;
	}
	cout << endl;
}
