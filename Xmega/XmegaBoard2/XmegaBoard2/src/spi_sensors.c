/*
 * spi_sensors.c
 *
 * Created: 7/23/2017 4:01:50 PM
 *  Author: rgw3d
 */ 

#include "spi_sensors.h"

void init_thermo_sensors(void){	

	//Init pins first


	//Chip select pin PF0
	ioport_configure_port_pin(&PORTF, PIN0_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PF1
	ioport_configure_port_pin(&PORTF, PIN1_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PR1
	ioport_configure_port_pin(&PORTR, PIN1_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PF3
	ioport_configure_port_pin(&PORTF, PIN3_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	//The SS pin MUST be high. otherwise will switch into slave mode
	//CANNOT USE PF4
	//PF4
	ioport_configure_port_pin(&PORTF, PIN4_bm, IOPORT_PULL_UP | IOPORT_DIR_INPUT);
	
	//MOSI PF5 UNUSED. THIS PIN IS NOT BROKEN OUT ON THE XMEGA XPLAINED
	ioport_configure_port_pin(&PORTF, PIN5_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//MISO PF6
	ioport_configure_port_pin(&PORTF, PIN6_bm, IOPORT_DIR_INPUT);
	//SCK PF7
	ioport_configure_port_pin(&PORTF, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);



	//Init Spi interface

	spi_pf0.id = IOPORT_CREATE_PIN(PORTF, 0);
	spi_pf1.id = IOPORT_CREATE_PIN(PORTF, 1);
	spi_pr1.id = IOPORT_CREATE_PIN(PORTR, 1);
	spi_pf3.id = IOPORT_CREATE_PIN(PORTF, 3);

	spi_master_init(&SPIF);
	
	spi_master_setup_device(&SPIF, &spi_pf0, SPI_MODE_0, 1000000, 0);
	/*spi_master_setup_device(&SPIF, &spi_pf1, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pr1, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pf3, SPI_MODE_0, 1000000, 0);
	*/
	spi_enable(&SPIF);
	

}

int16_t thermo_external_temp(uint32_t v){
	
	if (v & 0x7) {
		// uh oh, a serious problem!
		return 0; 
	}

	if (v & 0x80000000) {
		// Negative value, drop the lower 18 bits and explicitly extend sign bits.
		v = 0xFFFFC000 | ((v >> 18) & 0x00003FFFF);
	}
	else {
		// Positive value, just drop the lower 18 bits.
		v >>= 18;
	}
  return v; //multiply by .25 to get true value. BBB can do this
}
int16_t thermo_internal_temp(uint32_t v){
	
	v >>= 4;

	// pull the bottom 11 bits off
	uint16_t internal = v & 0x7FF;
	// check sign bit!
	if (v & 0x800) {
		// Convert to negative value by extending sign and casting to signed type.
		int16_t tmp = 0xF800 | (v & 0x7FF);
		internal = tmp;
	}
 
	return internal;// LSB = 0.0625 degrees
}


uint32_t read_thermo(uint8_t device){
	struct spi_device * spi_dev;
	if(device == 0){
		spi_dev = &spi_pf0;
	}
	else if(device == 1){
		spi_dev = &spi_pf1;
	}
	else if(device == 2){
		spi_dev = &spi_pr1;
	}
	else{
		spi_dev = &spi_pf3;
	}

	uint32_t buffer = 0;

	spi_select_device(&SPIF, spi_dev);
	spi_read_packet(&SPIF, (uint8_t *)&buffer, 4);
	spi_deselect_device(&SPIF, spi_dev);

	return buffer;
}
