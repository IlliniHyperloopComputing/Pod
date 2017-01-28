#include "sensor.h"

#include "codec.h"
#include <sys/time.h>


#define MILLIVOLT_SENSITIVITY 2434.0

struct timeval start_time;
sensor::sensor(std::vector<status_message_ptr> * tmp_status_buff, bool test) {
    this->tmp_status_buff = tmp_status_buff;
	this->simulate = test;
	if(simulate){
		gettimeofday(&start_time, NULL);
		distance = 600;
	}

//    status_message_ptr smp(new status_message(STATUS_ERROR,error_msg));
    //tmp_status_buff->push_back(smp);
    tick =1;
    tmp_status_buff->push_back(status_message_ptr(new status_message(STATUS_MESSAGE,"Entering Sensor Init")));
    printf("Entering Sensor init\n");

    tmp_status_buff->push_back(status_message_ptr(new status_message(init_lev(),"Init lev")));
    tmp_status_buff->push_back(status_message_ptr(new status_message(init_a(),"Init a")));
    //tmp_status_buff->push_back(status_message_ptr(new status_message(init_brake_pressure(),"Init Brake pressure")));
    tmp_status_buff->push_back(status_message_ptr(new status_message(init_temps(),"Init Temps")));
    tmp_status_buff->push_back(status_message_ptr(new status_message(init_rpm(),"Init rpm")));
    tmp_status_buff->push_back(status_message_ptr(new status_message(init_tape_count(),"Init tape count")));

    tmp_status_buff->push_back(status_message_ptr(new status_message(STATUS_MESSAGE,"Exiting Sensor Init")));
    printf("Exiting Sensor init\n");
}

sensor::~sensor(){
    if(atomic_lev != NULL)
        delete[] atomic_lev;
    if(atomic_temps != NULL)
        delete[] atomic_temps;
    if(atomic_rpm != NULL)
        delete[] atomic_rpm;

    close(i2c_brake);
    close(i2c_thermo);
    close(i2c_rpm);
    close(i2c_tape);
    close(i2c_a);
}
int16_t starting_millivolts[3];

double sim_accel(){
	struct timeval now;
	gettimeofday(&now, NULL);
    double delta = now.tv_sec - start_time.tv_sec + (now.tv_usec - start_time.tv_usec) / 1000000.0;
	if(delta < 10) {
		return 0;
	} else if (delta < 20){
		return 2.1;
	} else {
		return 0;
	}
}

void sensor::update(){
	if(!simulate){
		//always update
		update_a();
		  //update_brake_pressure();
		update_tape_count();
		update_v();

		switch(tick){
			case 1:
				break;
			case 2:
				update_lev();
			   // update_esc();
				update_temp();
				update_rpm();
				
				break;
			case 3:
	//            update_v();
				  //update_tot();
				break;
			default:
				break;
		}
		
		tick = (tick %3)+1; //tick range: [1:3]
	} else {
		//simulation time	
		double sim = sim_accel();
/*		for(int i = 0; i < 3; i++){
			atomic_a[i] = sim;
            
		}
*/
        atomic_a[0] = 1;
        atomic_a[1] = sim;
        atomic_a[2] = 0;

	}
}




///////////
//Get 
std::atomic<double> *  sensor::get_atomic_lev(){
    return atomic_lev;
}
std::atomic<double> *  sensor::get_atomic_v(){
    return &atomic_v;
}
std::atomic<double> *  sensor::get_atomic_a(){
    return atomic_a;
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

std::atomic<double> * sensor::get_brake_pressure() {
	return &atomic_brake_pressure;
}

std::atomic<double> * sensor::get_atomic_tape_count(){
	return &atomic_tape_count;
}

std::atomic<double> * sensor::get_distance() {
	return &distance;
}


///////////
//Init
int sensor::init_lev(){
    atomic_lev = new std::atomic<double>[3];
    return 0;
}
/*
int sensor::init_v(){
    atomic_v   = new std::atomic<double>[3];
    return 0;
}*/
void sensor::recalibrate_accelerometer(){
	long totals[3] = {0, 0, 0};	
    int16_t counts[3] = {1, 1, 1};
	for(int i = 0; i < 100; i++){
		for(int j = 0; j < 3; j++){
				
			i2c_smbus_write_byte(i2c_a,j);	
			int16_t x = i2c_smbus_read_word_data(i2c_a,0);
       //     std::cout << "x = " << x << ", totals[j] = " << totals[j]  << std::endl;
            if(x > 7000 && x < 16000){
			    totals[j] += x;
                counts[j]++;
            }
		}
        usleep(10000);
	}
    //std::cout << "END CALIBRATION" << std::endl;
	for(int i = 0; i < 3; i++){
		starting_millivolts[i] = totals[i] / counts[i];
        std::cout << starting_millivolts[i] << std::endl;
        
	}
}

int sensor::init_a(){
    //atomic_a  = new std::atomic<double>[3];
    i2c_a = open_i2c(0x20);
    if(i2c_a<0) return -1;//return if error
		
	recalibrate_accelerometer();
	return 0;
}

int sensor::init_brake_pressure(){
	//TODO give it a different address
    i2c_brake = open_i2c(0x48);
    if(i2c_brake<0) return -1;//return if error
    //i2c_brake_adc = new ADS1115(i2c_brake);
    //i2c_brake_adc->setRate(ADS1115_RATE_475); //RATE 475 SPS
	//i2c_brake_adc->setGain(ADS1115_PGA_6P144);//GAIN of 6.144
	//i2c_brake_adc->setMultiplexer(ADS1115_MUX_P0_NG);//Pin 0
	//i2c_brake_adc->setMode(ADS1115_MODE_SINGLESHOT);//Mode SingleShot
    atomic_brake_pressure.store(0);
    return 0;
}
int sensor::init_temps(){
    atomic_temps = new std::atomic<double>[8];
    i2c_thermo = open_i2c(0x15);
    if(i2c_thermo<0) return -1;
    return 0;
}
int sensor::init_rpm(){
    atomic_rpm = new std::atomic<double>[6];
    i2c_rpm = open_i2c(0x16);
    if(i2c_rpm<0) return -1;
    return 0;
}

int sensor::init_tape_count(){
	i2c_tape = open_i2c(0x17);
    if(i2c_tape<0) return -1;
    return 0;
}


////////////
//Update
void sensor::update_lev(){
	//TODO Make it work

	for(int i = 4; i < 7; i++) {

		int16_t millivolts = i2c_smbus_read_word_data(i2c_a, i);
		if(millivolts > 4000) { 
			double val = millivolts;
			double height = ((val - 4200) * 16.0)/22100 + 4;	
			atomic_lev[i-4].store(height);
            std::cout << "millivolts : " << millivolts << ", height : " << height << std::endl;
		}
	}
}
void sensor::update_v(){
	//request deltas
	delta = i2c_smbus_read_word_data(i2c_tape, 1);
	//delta is milliseconds
	double d = delta;
	double velocity = 100.0 / d; //feet/milliseconds
	velocity *= velocity * 304.8; //convert to meters/second
	atomic_v.store(velocity);
	

}
void sensor::update_a(){
	for(int i = 0; i < 3; i++){
	
	
    	int16_t x = i2c_smbus_read_word_data(i2c_a,i);
        if(x > 7000 &&  x < 16000){
            double g = (x - starting_millivolts[i]) / MILLIVOLT_SENSITIVITY;
            atomic_a[i].store(g);

            std::cout << "i = " << i << ", Starting millivolts[i] = " << starting_millivolts[i] << ", G = " << g << ", millivolts = "<<  x << std::endl;
        }
	}
}
void sensor::update_brake_pressure(){ 
    double millivolts = 1;//i2c_brake_adc->getMilliVolts();
    //TODO: math that turns millivolts to pressure
    atomic_brake_pressure.store(millivolts);
}
void sensor::update_rpm(){
    for(int i = 0; i < 6; i++){
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
	i2c_smbus_write_byte(i2c_tape, 0);
	int val = i2c_smbus_read_word_data(i2c_tape, 0);
	int current_count = atomic_tape_count.load();
	if(current_count < val){
		//find the speed
	}
	atomic_tape_count.store(val);
	distance.store(val * 100);
		
}

void sensor::reset_sensors(){
	i2c_smbus_write_byte(i2c_tape, 0xff);
	//writes 0xff, which the arduino receives 
	distance.store(0);
	atomic_tape_count.store(0);
	recalibrate_accelerometer();
}

int sensor::open_i2c(int address){
    int i2c = 0;
	i2c = open("/dev/i2c-1", O_RDWR);
	if(i2c < 0){
        std::string error_msg;
        error_msg= boost::str(boost::format("Error opening i2c port 0x%x\tSystem call open()\n") %address);
        status_message_ptr smp(new status_message(STATUS_ERROR,error_msg));
        tmp_status_buff->push_back(smp);

        //print out message
        fprintf(stderr, "Error opening i2c port 0x%x\tSystem call open()\n",address);
        return -1;
	}
	if( ioctl( i2c, I2C_SLAVE, address) < 0){
        std::string error_msg;
        error_msg= boost::str(boost::format("Error setting i2c address 0x%x\tSystem call ioctl()\n") %address);
        status_message_ptr smp(new status_message(STATUS_ERROR,error_msg));
        tmp_status_buff->push_back(smp);
        fprintf(stderr, "Error setting i2c address 0x%x\tSystem call ioctl()\n",address);
		return -1;
	}
	return i2c;
}

