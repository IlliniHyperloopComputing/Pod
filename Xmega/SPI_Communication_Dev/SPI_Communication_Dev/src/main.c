
#include <asf.h>
#include <avr/io.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>

//Spi to BBB
#include "spi_to_bbb.h"

//Using crc-ccitt
#include <stdbool.h>
#include <assert.h>

//Sensor data storage
//spi_to_bbb externs these variables
uint8_t state = 0;
uint8_t sensor_status = 0;
uint8_t sensor_data[SENSOR_DATA_SIZE] = {1,2,3,4,5,6,0,0,0,0,0,0};

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

//RTC
uint32_t time1 = 0;
uint32_t time2 = 1;
uint32_t time3 = 2;



uint8_t init_i2c(void);
uint8_t init_i2c(void)
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
	
	twi_master_options_t opt4B = {
		.speed = 100000,
		.chip = 0x4B
	};
	twi_master_setup(&TWIF, &opt4B);
	
	
	/* 0b01000010 == 0x42
	 * 15:Operating status. Reading this has value. writing 0 does nothing
	 * 14:12 Input mux. 000=AIN0
	 * 11:9  PGA config. 001=+4.096V
	 * 8:Mode 0=Continuous-conversion
	 * 0b11100011 == 0x83
	 * 7:5 Data Rate: 111=860 SPS
	 * 4: Comparater mode. 0=default
	 * 3: Comparater polarity. 0=default
	 * 2: Comparater Latch. 0=Nonlatching comparater
	 * 1: Comparater queue and disable. 00=Assert after one conversion
	 **/
	const uint8_t adc_setup_bytes[] = {0x42, 0xe3};
	twi_package_t adc_setup_48;
	memset(&adc_setup_48,0,sizeof(twi_package_t));
	adc_setup_48.addr[0]		= 0x01;
	adc_setup_48.addr_length	= sizeof(uint8_t);
	adc_setup_48.chip			= 0x48;
	adc_setup_48.buffer			= (void *)adc_setup_bytes;
	adc_setup_48.length			= sizeof(adc_setup_bytes);
	
	twi_package_t adc_setup_49;
	memset(&adc_setup_49,0,sizeof(twi_package_t));
	adc_setup_49.addr[0]		= 0x01;
	adc_setup_49.addr_length	= sizeof(uint8_t);
	adc_setup_49.chip			= 0x49;
	adc_setup_49.buffer			= (void *)adc_setup_bytes;
	adc_setup_49.length			= sizeof(adc_setup_bytes);
	
	twi_package_t adc_setup_4A;
	memset(&adc_setup_4A,0,sizeof(twi_package_t));
	adc_setup_4A.addr[0]		= 0x01;
	adc_setup_4A.addr_length	= sizeof(uint8_t);
	adc_setup_4A.chip			= 0x4A;
	adc_setup_4A.buffer			= (void *)adc_setup_bytes;
	adc_setup_4A.length			= sizeof(adc_setup_bytes);
	
	twi_package_t adc_setup_4B;
	memset(&adc_setup_4B,0,sizeof(twi_package_t));
	adc_setup_4B.addr[0]		= 0x01;
	adc_setup_4B.addr_length	= sizeof(uint8_t);
	adc_setup_4B.chip			= 0x4B;
	adc_setup_4B.buffer			= (void *)adc_setup_bytes;
	adc_setup_4B.length			= sizeof(adc_setup_bytes);
	
	uint8_t error = 0;
	if(twi_master_write(&TWIF, &adc_setup_48) != TWI_SUCCESS)
		error |= 1;
	if(twi_master_write(&TWIF, &adc_setup_49) != TWI_SUCCESS)
		error |= 2;
	if(twi_master_write(&TWIF, &adc_setup_4A) != TWI_SUCCESS)
		error |= 4;
	if(twi_master_write(&TWIF, &adc_setup_4B) != TWI_SUCCESS)
		error |= 8;
	
	return error;
}


int main (void)
{
	board_init();	//Init board
	sysclk_init();
	
	rtc_init();
	
	
	
	//rtc_set_time(1038);
	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	sei();            // enable global interrupts
	int sensor_error = init_i2c();
	state = 1;
	sensor_status = 52;
	
	
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		//_delay_ms(200);
		handle_spi_to_bbb();
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			uint8_t recieved_data[2];
			twi_package_t packet_read;
			memset(&packet_read, 0, sizeof(twi_package_t));
			packet_read.addr[0]			= 0x00;
			packet_read.addr_length		= sizeof(uint8_t);
			packet_read.chip			= 0x48;
			packet_read.buffer			= recieved_data;
			packet_read.length			= 2;
			
			time1= rtc_get_time();
			
			//_delay_ms(1);
			if(twi_master_read(&TWIF, &packet_read) == TWI_SUCCESS){
				sensor_data[0] = recieved_data[1];
				sensor_data[1] = recieved_data[0];
				
			}
			
			
			packet_read.chip = 0x49;
			
			if(twi_master_read(&TWIF,  &packet_read) ==TWI_SUCCESS){
				sensor_data[2] = recieved_data[1];
				sensor_data[3] = recieved_data[0];
			}
			if(spi_isr) continue;
			
			packet_read.chip = 0x4A;
			if(twi_master_read(&TWIF,  &packet_read) ==TWI_SUCCESS){
				sensor_data[4] = recieved_data[1];
				sensor_data[5] = recieved_data[0];
			}
			if(spi_isr) continue;
			
			packet_read.chip = 0x4B;
			if(twi_master_read(&TWIF,  &packet_read) ==TWI_SUCCESS){
				sensor_data[6] = recieved_data[1];
				sensor_data[7] = recieved_data[0];
			}
			if(spi_isr) continue;
			
			
			time2 = rtc_get_time();
			time3 = time2-time1;
			sensor_data[8] = time3 >> 0;
			sensor_data[9] = time3 >> 8;
			sensor_data[10] = time3 >> 16;
			sensor_data[11] = time3 >> 24;
			
			
			spi_transfer = 0;
		}
	}
}