#ifndef STATESPACE_H
#define STATESPACE_H
struct StateSpace {
  double x[3]; //Position/Velocity/Accel
  //x[0] = x
  //x[1] = x' first derivative
  //x[2] = x'' second derivative
  
  double rpm; //RPM of disks

  double fM; //Force Motor
  double fD; //Force Drag
};
#endif
