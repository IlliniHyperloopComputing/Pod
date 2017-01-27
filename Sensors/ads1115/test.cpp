#define DEBUG_ADS1115
#include "ADS1115.h"
#include <iostream>
#include <string>
#include <atomic>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <map>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <chrono>
#include <stdint.h>

using namespace std;
int open_i2c(int address){
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


int main(){
	int address = open_i2c(0x48);
    uint16_t muxes[] = {ADS1115_MUX_P0_NG, ADS1115_MUX_P1_NG, ADS1115_MUX_P2_NG, ADS1115_MUX_P3_NG};
	ADS1115 adc(address);
    adc.setRate(ADS1115_RATE_475); 
	adc.setGain(ADS1115_PGA_4P096);
	adc.setMultiplexer(ADS1115_MUX_P0_NG);
	adc.setMode(ADS1115_MODE_CONTINUOUS);

	adc.setComparatorMode(ADS1115_COMP_MODE_HYSTERESIS);//standard mode
	adc.setComparatorPolarity(ADS1115_COMP_POL_ACTIVE_HIGH);//polarity is high
	adc.setComparatorLatchEnabled(ADS1115_COMP_LAT_LATCHING);//latch
	adc.setComparatorQueueMode(ADS1115_COMP_QUE_ASSERT1);//latch on 1 match
	adc.setComparatorHiThresh(44000,4096);
	//adc.setComparatorLoThresh(0,4096);

	int i = 0;
	while(1){
		cin >>	i;
		printf("millis: %.4f\n",adc.getMilliVolts());
	}

}

