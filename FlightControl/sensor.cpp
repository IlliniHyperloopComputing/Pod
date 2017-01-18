#include "sensor.h"
#include "codec.h"

sensor::sensor(){
    tick =1;
    printf("Entering Sensor init\n");
    init_x();
    init_z();
    init_lev();
    init_v();
    init_a();
    init_att();
    init_brake_pressure();
    init_temps();
    init_rpm();
    init_tape_count();
	init_distances();
    printf("Exiting Sensor init\n");
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
    if(atomic_tape_count != NULL)
        delete[] atomic_tape_count;

    if(i2c_brake_adc != NULL)
        delete i2c_brake_adc;
    close(i2c_brake);
    close(i2c_thermo);
    close(i2c_rpm);
    close(i2c_tape);
}

//
void sensor::update(){
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
			update_tape_count();
            
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

///////////
//Get 
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

std::atomic<double> * sensor::get_atomic_tape_count(){
	return atomic_tape_count;
}

std::atomic<double> * sensor::get_distances() {
	return distances;
}


///////////
//Init
void sensor::init_x(){
    atomic_x.store(1);
}
void sensor::init_z(){
    atomic_z.store(1);
}
void sensor::init_lev(){
    atomic_lev = new std::atomic<double>[2];
}
void sensor::init_v(){
    atomic_v   = new std::atomic<double>[3];
}
void sensor::init_a(){
    atomic_a   = new std::atomic<double>[3];
}
void sensor::init_att(){
    atomic_att = new std::atomic<double>[3];
}
void sensor::init_brake_pressure(){
    i2c_brake = open_i2c(0x48);
    if(i2c_brake<0) return;//return if error
    i2c_brake_adc = new ADS1115(i2c_brake);
    i2c_brake_adc->setRate(ADS1115_RATE_475); //RATE 475 SPS
	i2c_brake_adc->setGain(ADS1115_PGA_6P144);//GAIN of 6.144
	i2c_brake_adc->setMultiplexer(ADS1115_MUX_P0_NG);//Pin 0
	i2c_brake_adc->setMode(ADS1115_MODE_SINGLESHOT);//Mode SingleShot
    atomic_brake_pressure.store(0);
}
void sensor::init_temps(){
    atomic_temps = new std::atomic<double>[8];
    i2c_thermo = open_i2c(0x15);
}
void sensor::init_rpm(){
    atomic_rpm = new std::atomic<double>[4];
    i2c_rpm = open_i2c(0x16);
}

void sensor::init_tape_count(){
	atomic_tape_count = new std::atomic<double>[4];
	i2c_tape = open_i2c(0x17);
}
void sensor::init_distances(){
	distances = new std::atomic<double>[4];
    for(int i = 0; i < 4; i++){
        distances[i] = 0;
        last_times[i] = std::chrono::high_resolution_clock::now();

    }
	remain_1000 = false;
	remain_500 = false;
}


////////////
//Update
void sensor::update_x(){
    //atomic_x.store(atomic_x.load()+1);
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
    double millivolts = i2c_brake_adc->getMilliVolts();
    //TODO: math that turns millivolts to pressure
    atomic_brake_pressure.store(millivolts);
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

void sensor::update_tape_count(){
	/*for(int i = 0; i < 1; i++){
	    int val = 0;
        i2c_smbus_write_byte(i2c_tape,i);
        val = i2c_smbus_read_word_data(i2c_tape,i);
		double oldCount = atomic_tape_count[i].load();
       	atomic_tape_count[i].store(val);

		time_t last = last_times[i];
		time_t now = std::chrono::high_resolution_clock::now();
		auto delta = now - last;
		auto max_delay = std::chrono::duration<long long int, std::ratio<1ll, 1000000000ll> >(5000);
        std::cout << "Distances[" << i << "] = " << distances[i].load() <<"val: "<<val << " old_count: " << oldCount  << " delta: " << delta.count() << std::endl;
		if(val > oldCount){
            std::cout << "FOUND ONE MY GOD" << std::endl;
            last_times[i] = now;
            if(delta.count() > max_delay.count()){
			    distances[i].store(distances[i].load() + 100);
           
		    } else if(!remain_1000 && !remain_500){
                std::cout << "1000 feet left!" << std::endl;
			    remain_1000 = true;
			    distance_at_1000 = distances[i].load();
		    } else if(remain_1000 && !remain_500 && distances[i].load() > distance_at_1000){
                std::cout << "500 feet left!" << std::endl;
			    remain_500 = true;
		    }	
        }


	}
    */
}

int sensor::open_i2c(int address){
    int i2c = 0;
	i2c = open("/dev/i2c-1", O_RDWR);
	if(i2c < 0){
       fprintf(stderr, "Error opening i2c port 0x%x\n\tSystem call open()\n",address);
        return -1;
	}
	if( ioctl( i2c, I2C_SLAVE, address) < 0){
        fprintf(stderr, "Error setting i2c address 0x%x\n\tSystem call ioctl()\n",address);
		return -1;
	}
	return i2c;
}

