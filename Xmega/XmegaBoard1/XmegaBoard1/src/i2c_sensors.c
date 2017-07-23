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

int8_t set_adc_mux(TWI_t * twi, uint8_t chip, uint8_t mux){
	
	adc_write.chip = chip;
	adc_write_bytes[0] = mux;	
	
	return twi_master_write(twi, &adc_write);
}

inline int8_t read_adc(TWI_t * twi, uint8_t chip, void * buff){
	
	adc_read.chip = chip;
	adc_read.buffer = (void *)buff;

	return twi_master_read(twi, &adc_read);
}


