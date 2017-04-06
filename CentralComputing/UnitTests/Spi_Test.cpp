#include "Spi.h"
#include "Spi_Test.h"
#include "Utils.h"

int Spi_Test::test(int argc, char** argv){
  uint8_t bpi[] = {2,2,2,2,2,2,2,2,2,2};
  Xmega_Setup x1 = {"/dev/spidev1.0", 10, bpi, 500000, 8};
  Xmega_Setup x2 = {"/dev/spidev1.1", 10, bpi, 500000, 8};
  Spi spi(&x1, &x2);

  Xmega_Transfer xt = {X_C_NONE, X_C_NONE, X_R_SENSOR, X_R_SENSOR};
  spi.transfer(xt);

  for(int i = 0; i < 10; i++){
    print_debug("got data: idx: %x\t\t data:%x\n",i, spi.get_data(0,i));
  }

  return 0;
}

Spi_Test::Spi_Test(){
  name = "Spi Test";
  cmd  = "spi";
}
