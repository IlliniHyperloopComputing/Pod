#include "Unit_Test.h"
#include "StateMachine_Test.h"
#include "Spi_Test.h"

int Unit_Test::test(int argc, char** argv){

  //
  //ADD NEW TESTS HERE. FOLLOW SAME PATTERN
  //
  Unit_Test* test_list[] = { new StateMachine_Test(), new Spi_Test() };
  int length = 2;
  //
  //


	vector<string> args(argv, argv + argc);

  if(argc < 2 || args[1] == "help"){
    display_help(test_list, length);
    
    //clean up memory
    for(int j=0;j<length;j++){delete test_list[j];};
    return 0;
  }

  string command = argv[1];
  for(int i = 0; i < length; i++){
    if(test_list[i]->cmd == command){
      //Call the appropriate test
      test_list[i]->test(argc-1, argv+1); 

      //clean up memory
      for(int j=0;j<length;j++){delete test_list[j];};

      return 0;
    }
  }

  cout << endl;
  cout << "NO MATCHING COMMANDS FOUND" << endl;
  cout << endl;

  //clean up memory
  for(int j=0;j<length;j++){delete test_list[j];};
  return 0;
}


void Unit_Test::display_help(Unit_Test* test_list[], int len){
	cout << "==============================" << endl;
	cout << "ILLINI HYPERLOOP TESTING SUITE " << endl;
	cout << "==============================" << endl;
  cout << endl;
  cout << "Usage: ./exe command [optional arguments based on command]" << endl;
	cout << "Commands:" << endl;
  for(int i = 0; i< len; i++){
    cout << "\t" << test_list[i]->cmd << ", "<< test_list[i]->name<< endl;
  }

}

//Destructor that does nothing
Unit_Test::~Unit_Test(){

}
