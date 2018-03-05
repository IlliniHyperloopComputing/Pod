#ifndef SIM
#include "Xmega.h"

//Static data intialization must happen here. pretty lame if you ask me

//I declare these arrays and then use their address below simply because I like 
//the {} array syntax. 


Xmega::Xmega() {
}

Xmega::~Xmega() {
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
