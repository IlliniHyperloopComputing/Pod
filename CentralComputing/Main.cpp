#include "Spi.h"
#include "Unit_Test.h"

int main(int argc, char** argv) {

  system("cpufreq-set -f 1000MHz");

  #ifdef TEST_POD
    Unit_Test ut;
    ut.test(argc, argv);
  #else
    //TODO: Define normal run mode
  #endif


  return 0;
}
