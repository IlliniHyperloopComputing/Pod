#include "Spi.h"
#include "Unit_Test.h"
#include "Pod.h"


int main(int argc, char** argv) {

  //Set frequency to max
  system("cpufreq-set -f 1000MHz");

  //Setup GPIO 60 so we can read it in
  system("echo \"gpio60\" > /sys/gpio/gpio");
  system("echo \"in\" > /sys/gpio/gpio60/direction");

  #ifdef TEST_POD
    Unit_Test ut;
    ut.test(argc, argv);
  #else
    //TODO: Define normal run mode
    printf("Running\n");	
    pod(argc, argv);

  #endif


  return 0;
}
