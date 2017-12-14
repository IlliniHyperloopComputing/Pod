#ifndef SIM
#include "Xmega.h"

Xmega::Xmega(Spi * s) : spi(s){

}


void Xmega::write(Xmega_Command command) {
  q.push(command);
}

Xmega_Command Xmega::transfer() {
  Xmega_Command command;
  if(q.empty())
    command = X_NONE;
  else {
    command = q.front();
    q.pop();
  }
  //TODO construct transfer object, call spi->transfer(transfer)

  return command;

}
#endif
