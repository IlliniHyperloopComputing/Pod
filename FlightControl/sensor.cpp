#include "sensor.h"

sensor::sensor(){
    x=0;    
    z=0;
    lev = new double[2];
    v   = new double[3];
    a   = new double[3];
    brake_pressure = 0;
    esc = new double[4];
    tot = new double[4];
}

sensor::~sensor(){
    if(lev != NULL)
        delete[] lev;
    if(v != NULL)
        delete[] v;
    if(a != NULL)
        delete[] a;
    if(esc != NULL)
        delete[] esc;
    if(tot != NULL)
        delete[] tot;
}

void sensor::update(){
    //update selected sensors here and store values

}

double * sensor::get_x(){
    return &x;
}
double * sensor::get_z(){
    return &z;
}
double * sensor::get_lev(){
    return lev;
}
double * sensor::get_v(){
    return v;
}
double * sensor::get_a(){
    return a;
}
double * sensor::get_brake(){
    return &brake_pressure;
}
double * sensor::get_esc(){
    return esc;
}
double * sensor::get_tot(){
    return tot;
}

