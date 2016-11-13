#include "sensor.h"
#include <string.h>
#include "codec.h"


sensor::sensor(){
    tick =1;
    
    atomic_x.store(1);// = new std::atomic<double>(1);
    atomic_z.store(1);
    atomic_lev = new std::atomic<double>[2];
    atomic_v   = new std::atomic<double>[3];
    atomic_a   = new std::atomic<double>[3];
    atomic_att = new std::atomic<double>[3];
    atomic_brake_pressure.store(3);
    atomic_temps = new std::atomic<double>[8];
    atomic_rpm = new std::atomic<double>[4];

    i2c_thermo = open_i2c(0x15);
    i2c_rpm = open_i2c(0x16);
    
}

sensor::~sensor(){
    if(atomic_lev != NULL)
        delete[] atomic_lev;
    if(atomic_v != NULL)
        delete[] atomic_v;
    if(atomic_a != NULL)
        delete[] atomic_a;
    if(atomic_att != NULL)
        delete[] atomic_att;
    if(atomic_temps != NULL)
        delete[] atomic_temps;
    if(atomic_rpm != NULL)
        delete[] atomic_rpm;
}

void sensor::update(){
    usleep(30000);
    //always update
//    update_x();
//    update_a();
//    update_brake_pressure();

    switch(tick){
        case 1:
//            update_v();
//            update_z();
            break;
        case 2:
 //           update_lev();
            //update_esc();
           // update_temp();
            update_rpm();
            
//            update_att();
            break;
        case 3:
//            update_v();
            //update_tot();
            break;
        default:
            break;
    }
    
    tick = (tick %3)+1; //tick range: [1:3]
}


/*double * sensor::get_x(){
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
double * sensor::get_att(){
    return att;
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
*/

std::atomic<double> *  sensor::get_atomic_x(){
    return &atomic_x;
}
std::atomic<double> *  sensor::get_atomic_z(){
    return &atomic_z;
}
std::atomic<double> *  sensor::get_atomic_lev(){
    return atomic_lev;
}
std::atomic<double> *  sensor::get_atomic_v(){
    return atomic_v;
}
std::atomic<double> *  sensor::get_atomic_a(){
    return atomic_a;
}
std::atomic<double> *  sensor::get_atomic_att(){
    return atomic_att;
}
std::atomic<double> *  sensor::get_atomic_brake(){
    return &atomic_brake_pressure;
}
std::atomic<double> *  sensor::get_atomic_rpm(){
    return atomic_rpm;
}
std::atomic<double> * sensor::get_atomic_temps(){
    return atomic_temps;
}

void sensor::update_x(){
    atomic_x.store(atomic_x.load()+1);
}

void sensor::update_z(){
    atomic_z.store(1);
}

void sensor::update_lev(){
    atomic_lev[0].store(1);
    atomic_lev[1].store(2);
}

void sensor::update_v(){
    atomic_v[0].store(1);
    atomic_v[1].store(3);
    atomic_v[2].store(5);

}

void sensor::update_a(){
    atomic_a[0].store(1);
    atomic_a[1].store(2);
    atomic_a[2].store(5);

}

void sensor::update_att(){
    atomic_att[0].store(1);
    atomic_att[1].store(2);
    atomic_att[2].store(5);
}

void sensor::update_brake_pressure(){
    atomic_brake_pressure.store(atomic_brake_pressure.load()+1);
}

void sensor::update_rpm(){
    for(int i = 0; i < 4; i++){
        int val = 0;
        i2c_smbus_write_byte(i2c_rpm,i);
        val = i2c_smbus_read_word_data(i2c_rpm,i);
        atomic_rpm[i].store(val);
    }
}

void sensor::update_temp(){
    for(int i = 0; i < 8; i++){
        unsigned char val = 0;
        i2c_smbus_write_byte(i2c_thermo,i);
        val = i2c_smbus_read_byte_data(i2c_thermo,i);
        //std::cout << val << std::endl;
        //std::cout << "Thermocouple " << i << " : " << (int)val << std::endl; 
        atomic_temps[i].store(val);
        
    }
}

int sensor::open_i2c(int address){
    int i2c = 0;
	i2c = open("/dev/i2c-1", O_RDWR);
	if(i2c < 0){
		std::cout << "Something went wrong opening the i2c port" <<std::endl;
        return -1;
	}
	if( ioctl( i2c, I2C_SLAVE, address) < 0){
		std::cout << "Failed to set i2c (address: " << address << ") slave address" <<std::endl;
		return -1;
	}
	return i2c;
}
