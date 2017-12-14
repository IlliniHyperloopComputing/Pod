#ifdef SIM
#include "Xmega.h"

Xmega::Xmega(Spi * s) {

}

void Xmega::write(Xmega_Command command) {
}

Xmega_Command Xmega::transfer(){
  return X_NONE;
}
#endif
