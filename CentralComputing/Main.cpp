#include "Spi.h"
#include "Unit_Test.h"

int main(int argc, char** argv) {

  #ifdef TEST_POD
    Unit_Test ut;
    ut.test(argc, argv);
  #else
    //TODO: Define normal run mode
  #endif


  return 0;
}
