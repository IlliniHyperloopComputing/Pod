#include "sensor.h"
#include <string.h>
#include "codec.h"


sensor::sensor(){
    tick =1;
    /*
    x=0;    
    z=0;
    lev = new double[2];
    v   = new double[3];
    a   = new double[3];
    att = new double[3];
    brake_pressure = 0;
    esc = new double[4];
    tot = new double[4];
       */

    
    atomic_x.store(1);// = new std::atomic<double>(1);
    atomic_z.store(1);
    atomic_lev = new std::atomic<double>[2];
    atomic_v   = new std::atomic<double>[3];
    atomic_a   = new std::atomic<double>[3];
    atomic_att = new std::atomic<double>[3];
    atomic_brake_pressure.store(3);
    atomic_esc = new std::atomic<double>[4];
    atomic_tot = new std::atomic<double>[4];
    open_i2c();
    open_i2c_address(0x15);
    
}

sensor::~sensor(){
    /*if(lev != NULL)
        delete[] lev;
    if(v != NULL)
        delete[] v;
    if(a != NULL)
        delete[] a;
    if(att != NULL)
        delete[] att;
    if(esc != NULL)
        delete[] esc;
    if(tot != NULL)
        delete[] tot;
    */
    if(atomic_lev != NULL)
        delete[] atomic_lev;
    if(atomic_v != NULL)
        delete[] atomic_v;
    if(atomic_a != NULL)
        delete[] atomic_a;
    if(atomic_att != NULL)
        delete[] atomic_att;
    if(atomic_esc != NULL)
        delete[] atomic_esc;
    if(atomic_tot != NULL)
        delete[] atomic_tot;
}

void sensor::update(){
    //always update
    update_x();
    update_a();
    update_brake_pressure();

    switch(tick){
        case 1:
            update_v();
            update_z();
            break;
        case 2:
            update_lev();
            //update_esc();
            update_temp();
            update_att();
            break;
        case 3:
            update_v();
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
std::atomic<double> *  sensor::get_atomic_esc(){
    return atomic_esc;
}
std::atomic<double> *  sensor::get_atomic_tot(){
    return atomic_tot;
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

/*
void sensor::update_esc(){
    atomic_esc[0].store(3);
    atomic_esc[1].store(50);
    atomic_esc[2].store(57);
    atomic_esc[3].store(78);
}

void sensor::update_tot(){
    atomic_tot[0].store(3);
    atomic_tot[1].store(50);
    atomic_tot[2].store(57);
    atomic_tot[3].store(78);
}*/


void sensor::update_temp(){
/*
    std::cout << "Updating temp" << std::endl;
    char buf[3];

    memset(buf, 0, 3);
    int bytesread = read(i2c, buf, 3);

    std::cout << "Bytes read : " <<bytesread<< std::endl;

    for(int i = 0; i < 3; i++){
       std::cout << "Thermocouple " << i << " : " << (int)buf[i] << std::endl; 
    }
*/

/*
    unsigned char buf[8];
    memset(buf, 0, 8);
     
    i2c_smbus_read_block_data(i2c, 0x00, buf);
    
    for(int i = 0; i < 8; i++){
       std::cout << "Thermocouple " << i << " : " << (int)buf[i] << std::endl; 
    }
*/

    unsigned char val = 0;

    val = i2c_smbus_read_byte(i2c);
    std::cout << (unsigned int)val << std::endl;
    
}


int sensor::open_i2c(){
	i2c = open("/dev/i2c-1", O_RDWR);
	if(i2c < 0){
		std::cout << "Something went wrong opening the i2c port" <<std::endl;
        return -1;
	}
    return 1;
}

//-1 == fail
//1 == good
int sensor::open_i2c_address(int address){
	if( ioctl( i2c, I2C_SLAVE, address) < 0){
		std::cout << "Failed to set i2c (address: " << address << ") slave address" <<std::endl;
	//	cout << "Error Code: " << cerr << endl;
		return -1;
	}
	
	return 1;
}
