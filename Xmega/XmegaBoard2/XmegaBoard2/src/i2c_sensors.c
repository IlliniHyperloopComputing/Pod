/*
 * i2c_sensors.c
 *
 * Created: 7/22/2017 4:02:09 PM
 *  Author: rgw3d
 */ 

#include "i2c_sensors.h"

void init_chip(TWI_t *twi, uint8_t chip){
	
	twi_master_options_t opt = {
		.speed = 100000,
		.chip = chip
	};
	twi_master_setup(twi, &opt);
	
}

const uint8_t calValue[] = {0x10, 0x00}; //4096, with MSB first
uint16_t current_read_buffer = 0;
	
int8_t init_current(TWI_t *twi, uint8_t chip){
	
	//initialize the chip on the specified bus
	init_chip(twi,chip);
	
	//Calibrating to 32V 2A. Refer to github listed below
	
	//Set Calibration
	current_write.addr[0]		= INA219_REG_CALIBRATION;
	current_write.addr_length	= sizeof(uint8_t);
	current_write.chip			= chip;
	current_write.buffer		= (void *)calValue;
	current_write.length		= sizeof(calValue);
	
	twi_master_write(twi, &current_write);
	
	
	//Set Configuration
	const uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
	INA219_CONFIG_GAIN_8_320MV |
	INA219_CONFIG_BADCRES_12BIT |
	INA219_CONFIG_SADCRES_12BIT_1S_532US |
	INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
	uint8_t configValue[] = {(config>>8)&0xff, config & 0xff};
	//ina219_currentDivider_mA = 10; This is for the BBB really. Just putting it hear as a reference
	
	current_write.addr[0]		= INA219_REG_CONFIG;
	current_write.addr_length	= sizeof(uint8_t);
	current_write.chip			= chip;
	current_write.buffer		= (void *)configValue;
	current_write.length		= sizeof(configValue);
	
	twi_master_write(twi, &current_write);
	
	//Set this variable back up. The Calibration can get wiped sometimes, so it needs to be re-written before a read can happen
	current_write.addr[0]		= INA219_REG_CALIBRATION;
	current_write.addr_length	= sizeof(uint8_t);
	current_write.chip			= chip;
	current_write.buffer		= (void *)calValue;
	current_write.length		= sizeof(calValue);
	
	//Set up read variable
	current_read.addr[0]		= INA219_REG_CURRENT;
	current_read.addr_length	= sizeof(uint8_t);
	current_read.chip			= chip;
	current_read.buffer			= (void *)current_read_buffer;
	current_read.length			= sizeof(current_read_buffer);
		
	return 0;
	
}

inline int8_t read_current(TWI_t *twi, uint8_t chip, void * buff) {

	// Sometimes a sharp load will reset the INA219, which will
	// reset the cal register, meaning CURRENT and POWER will
	// not be available ... avoid this by always setting a cal
	// value even if it's an unfortunate extra step
	twi_master_write(twi, &current_write);

	// Now we can safely read the CURRENT register!
	current_read.chip = chip;
	current_read.buffer = buff;
	
	return twi_master_read(twi, &current_read);
}
	
uint8_t adc_write_bytes[] = {0xc3, 0xe3};
uint16_t adc_read_buffer = 0;

int8_t init_adc(TWI_t *twi, uint8_t chip, uint8_t type_of_adc){
	
	//initialize the chip on the specified bus
	init_chip(twi, chip);
	
	adc_write_bytes[0] = type_of_adc;
	
	//set up and write config settings
	adc_write.addr[0]		= 0x01;
	adc_write.addr_length	= sizeof(uint8_t);
	adc_write.chip			= chip;
	adc_write.buffer		= (void *)adc_write_bytes;
	adc_write.length		= sizeof(adc_write_bytes);

	twi_master_write(twi, &adc_write);

	//read configs back to make sure that they match
	adc_read.addr[0]		= 0x01;
	adc_read.addr_length	= sizeof(uint8_t);
	adc_read.chip			= chip;
	adc_read.buffer			= (void *)adc_read_buffer;
	adc_read.length			= sizeof(adc_read_buffer);

	twi_master_read(twi, &adc_read);

	//Set this back to 0x00 because read_adc expects it to be this value
	adc_read.addr[0]		= 0x00;

	//Check if mux settings match. check this in particular because there are 1 and 0 values
	//Reading something that is all 1s might not work if the bus is already pulled up
	//0 indicates good value
	//1 indicates something is wrong
	return !((adc_read_buffer & 0x7000) == 0x0400);

}

inline int8_t set_adc_mux(TWI_t * twi, uint8_t chip, uint8_t mux){
	
	adc_write.chip = chip;
	adc_write_bytes[0] = mux;	
	
	return twi_master_write(twi, &adc_write);
}

inline int8_t read_adc(TWI_t * twi, uint8_t chip, void * buff){
	
	adc_read.chip = chip;
	adc_read.buffer = (void *)buff;

	return twi_master_read(twi, &adc_read);
}


