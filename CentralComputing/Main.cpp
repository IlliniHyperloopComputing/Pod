#include "Sensor_Test.h"
int main(int argc, char** argv) {

  #ifdef TEST_POD
    Sensor_Test::start_test(argc, argv);
    return 0;
  #else
    //Define normal run mode
  #endif


  return 0;
}
