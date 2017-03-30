#include "Spi.h"
#include "Spi_Test.h"

int Spi_Test::test(int argc, char** argv){
  uint8_t bpi[] = {2,2};
  Xmega_Setup x1 = {"/dev/spidev1.0", 2, bpi, 500000, 8};
  Xmega_Setup x2 = {"/dev/spidev1.1", 2, bpi, 500000, 8};
  Spi spi(&x1, &x2);

  return 0;
}

Spi_Test::Spi_Test(){
  name = "Spi Test";
  cmd  = "spi";
}
