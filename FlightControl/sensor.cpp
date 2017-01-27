#include "sensor.h"
#include "codec.h"

sensor::sensor(std::vector<status_message_ptr> * tmp_status_buff){
    this->tmp_status_buff = tmp_status_buff;

//    status_message_ptr smp(new status_message(STATUS_ERROR,error_msg));
    //tmp_status_buff->push_back(smp);
    tick =1;
    tmp_status_buff->push_back(status_message_ptr(new status_message(STATUS_MESSAGE,"Entering Sensor Init")));
    printf("Entering Sensor init\n");

    tmp_status_buff->push_back(status_message_ptr(new status_message(init_lev(),"Init lev")));
    tmp_status_buff->push_back(status_message_ptr(new status_message(init_v(),"Init v")));
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
    if(atomic_v != NULL)
        delete[] atomic_v;
    if(atomic_a != NULL)
        delete[] atomic_a;
    if(atomic_temps != NULL)
        delete[] atomic_temps;
    if(atomic_rpm != NULL)
        delete[] atomic_rpm;

    if(i2c_brake_adc != NULL)
        delete i2c_brake_adc;
    if(i2c_a_adc!= NULL)
        delete i2c_a_adc;
    close(i2c_brake);
    close(i2c_thermo);
    close(i2c_rpm);
    close(i2c_tape);
    close(i2c_a);
}

//
void sensor::update(){
    //always update
      update_a();
      //update_brake_pressure();

    switch(tick){
        case 1:
//            update_v();
            break;
        case 2:
 //           update_lev();
            //update_esc();
            update_temp();
            update_rpm();
			update_tape_count();
            
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
std::atomic<double> *  sensor::get_atomic_lev(){
    return atomic_lev;
}
std::atomic<double> *  sensor::get_atomic_v(){
    return atomic_v;
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
int sensor::init_v(){
    atomic_v   = new std::atomic<double>[3];
    return 0;
}
int sensor::init_a(){
    atomic_a   = new std::atomic<double>[3];
    i2c_a = open_i2c(0x48);
    if(i2c_a<0) return -1;//return if error
    i2c_a_adc = new ADS1115(i2c_a,0x48);
    if(i2c_a_adc->testConnection()==0){
        printf("Something is wrong with init a\n");
    }
    
    //i2c_a_adc->setRate(ADS1115_RATE_475); //RATE 475 SPS
	//i2c_a_adc->setGain(ADS1115_PGA_6P144);//GAIN of 6.144
	//i2c_a_adc->setMultiplexer(ADS1115_MUX_P0_NG);//Pin 0
	//i2c_a_adc->setMode(ADS1115_MODE_SINGLESHOT);//Mode SingleShot
    atomic_brake_pressure.store(0);
    return 0;
}
int sensor::init_brake_pressure(){
    i2c_brake = open_i2c(0x48);
    if(i2c_brake<0) return -1;//return if error
    i2c_brake_adc = new ADS1115(i2c_brake,0x48);
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
    atomic_lev[0].store(1);
    atomic_lev[1].store(2);
    atomic_lev[2].store(3);
}
void sensor::update_v(){
    atomic_v[0].store(1);
    atomic_v[1].store(3);
    atomic_v[2].store(5);
}
void sensor::update_a(){
	i2c_a_adc->setMultiplexer(ADS1115_MUX_P0_NG);
    double x = i2c_a_adc->getMilliVolts();
    printf("milii x: %f\n", x);
	i2c_a_adc->setMultiplexer(ADS1115_MUX_P1_NG);
	x = (x + i2c_a_adc->getMilliVolts())/2.0;
	i2c_a_adc->setMultiplexer(ADS1115_MUX_P2_NG);
    double y = i2c_a_adc->getMilliVolts();
	i2c_a_adc->setMultiplexer(ADS1115_MUX_P3_NG);
    double z = i2c_a_adc->getMilliVolts();
    atomic_a[0].store(x);
    atomic_a[1].store(y);
    atomic_a[2].store(z);
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

	int val = i2c_smbus_read_word_data(i2c_tape, 0);
	atomic_tape_count.store(val);
	distance.store(val * 100);
		
}

void sensor::reset_tape_count(){
	i2c_smbus_write_byte(i2c_tape, 0xff);
	//writes 0xff, which the arduino receives 
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

