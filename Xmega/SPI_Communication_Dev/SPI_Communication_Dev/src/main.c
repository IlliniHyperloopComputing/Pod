
#include <asf.h>
#include <twi_master.h>
#include <avr/io.h>

//Spi to BBB
#include "spi_to_bbb.h"

//Using crc-ccitt
#include <stdbool.h>
#include <assert.h>

//Sensor data storage
//spi_to_bbb externs these variables
uint8_t state = 0;
uint8_t sensor_status = 0;
uint8_t sensor_data[SENSOR_DATA_SIZE] = {1,2,3,4,5,6};

//lock
uint8_t lock = 0;


void setUpI2C()
{
	twi_master_options_t opt48 = {
		.speed = 100000,
		.chip = 0x48
	};
	twi_master_setup(&TWIF, &opt48);
	
	twi_master_options_t opt49 = {
		.speed = 100000,
		.chip = 0x49
	};
	twi_master_setup(&TWIF, &opt49);
	
	twi_master_options_t opt4A = {
		.speed = 100000,
		.chip = 0x4A
	};
	twi_master_setup(&TWIF, &opt4A);
	
	const uint8_t adc_setup_bytes[] = {0x42, 0x83};
	twi_package_t adc_setup_48 = {
		.addr			= 0x01,
		.addr_length	= sizeof(uint8_t),
		.chip			= 0x48,
		.buffer			= (void *)adc_setup_bytes,
		.length			= sizeof(adc_setup_bytes)
	};
	twi_package_t adc_setup_49 = {
		.addr			= 0x01,
		.addr_length	= sizeof(uint8_t),
		.chip			= 0x49,
		.buffer			= (void *)adc_setup_bytes,
		.length			= sizeof(adc_setup_bytes)
	};
	twi_package_t adc_setup_4A = {
		.addr			= 0x01,
		.addr_length	= sizeof(uint8_t),
		.chip			= 0x4A,
		.buffer			= (void *)adc_setup_bytes,
		.length			= sizeof(adc_setup_bytes)
	};
	
	
	while(twi_master_write(&TWIF, &adc_setup_48) != TWI_SUCCESS);
	while(twi_master_write(&TWIF, &adc_setup_49) != TWI_SUCCESS);
	while(twi_master_write(&TWIF, &adc_setup_4A) != TWI_SUCCESS);
	
}


int main (void)
{
	board_init();	//Init board
	init_spi_to_bbb();	//Setup SPI on Port C
	setUpI2C();
	state = 1;
	sensor_status = 52;
	
	sei();            // enable global interrupts
	
	
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		
		handle_spi_to_bbb();
		
		if(lock == 0){//Do anything that is not SPI related
			uint8_t recieved_data[2];
			twi_package_t packet_read = {
				.addr			= 0x00,
				.addr_length	= sizeof(uint8_t),
				.chip			= 0x48,
				.buffer			= recieved_data,
				.length			= 2
				
			};
			
			if(twi_master_read(&TWIF, &packet_read) == TWI_SUCCESS){
				sensor_data[0] = recieved_data[1];
				sensor_data[1] = recieved_data[0];
			}
			
			packet_read.chip = 0x49;
			if(twi_master_read(&TWIF,  &packet_read) ==TWI_SUCCESS){
				sensor_data[2] = recieved_data[1];
				sensor_data[3] = recieved_data[0];
			}
			
			packet_read.chip = 0x4A;
			if(twi_master_read(&TWIF,  &packet_read) ==TWI_SUCCESS){
				sensor_data[4] = recieved_data[1];
				sensor_data[5] = recieved_data[0];
			}
			
			lock = 1;
			
			
		}
	}
	
}