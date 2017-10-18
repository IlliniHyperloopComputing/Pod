#ifdef SIM

#include "Sensor.h"
Sensor::Sensor(Xmega * xm) : xmega(xm){

}

Data * Sensor::get_data(Data_ID id){
  return NULL;
}

void Sensor::update_buffers() {
}
#endif
