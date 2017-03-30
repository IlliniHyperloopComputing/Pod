#include "Spi.h"
#include "Sensor_Test.h"

int main(int argc, char** argv) {

  #ifdef TEST_POD
    Sensor_Test::start_test(argc, argv);
    return 0;
  #else
    //Define normal run mode
    uint8_t bpi[] = {2,2};
    Xmega_Setup x1 = {"/dev/spidev1.0", 2, bpi, 500000, 8};
    Xmega_Setup x2 = {"/dev/spidev1.1", 2, bpi, 500000, 8};
    Spi spi(&x1, &x2);
      
      
  #endif


  return 0;
}
