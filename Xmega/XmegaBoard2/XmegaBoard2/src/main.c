
#include <asf.h>
#include <avr/io.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>
#define APROX_HALF_SECOND 151000

//Spi to BBB
#include "spi_to_bbb.h"

//Using crc-ccitt
#include <stdbool.h>
#include <assert.h>

//Sensor data storage
//spi_to_bbb externs these variables
uint8_t state = 0;
uint8_t sensor_status = 0;
uint8_t sensor_data[SENSOR_DATA_SIZE] = {0};
/*
index to value
0,1 == y  i2c 
2,3 == z  i2c 
4,5 == RH0 i2c
6,7 == RH1 i2c 
8,9 == RH2 i2c
10,11 == Battery i2c
12,13 == Battery ..
14,15 == Battery
16,17 == Battery
18,19 == Battery
20,21 == Battery
22,23 == Battery ..
24,25 == Battery i2c
26,27 == Thermo0 Spi
28,29 == Thermo1
30,31 == Thermo2
32,33 == Thermo3
34,35 == Thermo3
36    == RetroReflective  Interrupt
*/

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

//RTC
uint32_t time1 = 0;
uint32_t time2 = 1;
uint32_t time3 = 2;
uint32_t time4 = 0;
uint32_t time5 = 0;

struct spi_device spi_pf0 = {
	.id = IOPORT_CREATE_PIN(PORTF, 0)
};
struct spi_device spi_pf1 = {
	.id = IOPORT_CREATE_PIN(PORTF, 1)
};
struct spi_device spi_pf2 = {
	.id = IOPORT_CREATE_PIN(PORTF, 2)
};
struct spi_device spi_pf3 = {
	.id = IOPORT_CREATE_PIN(PORTF, 3)
};
struct spi_device spi_pr0 = {
	.id = IOPORT_CREATE_PIN(PORTR, 0)
};

void spi_init_pins(void)
{
	//Chip select pin PF0
	ioport_configure_port_pin(&PORTF, PIN0_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PF1
	ioport_configure_port_pin(&PORTF, PIN1_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PF2
	ioport_configure_port_pin(&PORTF, PIN2_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PF3
	ioport_configure_port_pin(&PORTF, PIN3_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PR0
	ioport_configure_port_pin(&PORTR, PIN0_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	//The SS pin MUST be high. otherwise will switch into slave mode
	//CANOT USE PF4
	//PF4
	ioport_configure_port_pin(&PORTF, PIN4_bm, IOPORT_PULL_UP | IOPORT_DIR_INPUT);
	
	//MISO PF6
	ioport_configure_port_pin(&PORTF, PIN6_bm, IOPORT_DIR_INPUT);
	//SCK PF7
	ioport_configure_port_pin(&PORTF, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
}
void spi_init_module(void)
{
	spi_master_init(&SPIF);
	
	spi_master_setup_device(&SPIF, &spi_pf0, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pf1, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pf2, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pf3, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pr0, SPI_MODE_0, 1000000, 0);
	
	spi_enable(&SPIF);
}

uint8_t init_i2c(void);
uint8_t init_i2c(void)
{
	twi_master_options_t opt48 = {
		.speed = 100000,
		.chip = 0x48
	};
	twi_master_setup(&TWIC, &opt48);//Reads Y, Z values
	twi_master_setup(&TWIE, &opt48);//Reads Battery voltages
	
	twi_master_options_t opt49 = {
		.speed = 100000,
		.chip = 0x49
	};
	twi_master_setup(&TWIC, &opt49);//Reads RHS
	twi_master_setup(&TWIE, &opt49);//Reads Battery Voltages
	
	twi_master_options_t opt4A = {
		.speed = 100000,
		.chip = 0x4A
	};
	twi_master_setup(&TWIC, &opt4A);//Reads RHS
	twi_master_setup(&TWIE, &opt4A);//Reads Battery Voltages
	
	twi_master_options_t opt4B = {
		.speed = 100000,
		.chip = 0x4B
	};
	twi_master_setup(&TWIC, &opt4B);//Reads RHS
	twi_master_setup(&TWIE, &opt4A);//Reads Battery Voltages
	
	
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
	//STREAMING adcs:
	const uint8_t adc_setup_bytes_stream[] = {0x42, 0xe3};
	twi_package_t adc_setup_stream;
	memset(&adc_setup_stream,0,sizeof(twi_package_t));
	adc_setup_stream.addr[0]		= 0x01;
	adc_setup_stream.addr_length	= sizeof(uint8_t);
	adc_setup_stream.chip			= 0x49;
	adc_setup_stream.buffer			= (void *)adc_setup_bytes_stream;
	adc_setup_stream.length			= sizeof(adc_setup_bytes_stream);
	twi_master_write(&TWIC, &adc_setup_stream);
	
	adc_setup_stream.chip			= 0x4A;
	twi_master_write(&TWIC, &adc_setup_stream);
	
	adc_setup_stream.chip			= 0x4B;
	twi_master_write(&TWIC, &adc_setup_stream);
	
	//NON STREAMING
	const uint8_t adc_setup_bytes_non[] = {0x43, 0xe3};
	twi_package_t adc_setup_non;
	memset(&adc_setup_non,0,sizeof(twi_package_t));
	adc_setup_non.addr[0]			= 0x01;
	adc_setup_non.addr_length		= sizeof(uint8_t);
	adc_setup_non.chip				= 0x48;
	adc_setup_non.buffer			= (void *)adc_setup_bytes_non;
	adc_setup_non.length			= sizeof(adc_setup_bytes_non);
	twi_master_write(&TWIC, &adc_setup_stream);
	twi_master_write(&TWIE, &adc_setup_stream);
	
	adc_setup_non.chip				= 0x49;
	twi_master_write(&TWIE, &adc_setup_stream);
	
	adc_setup_non.chip				= 0x4A;
	twi_master_write(&TWIE, &adc_setup_stream);
	
	adc_setup_non.chip				= 0x4B;
	twi_master_write(&TWIE, &adc_setup_stream);
	
	return 0;
}

//Pass in 32 bit value from temperature sensor to decode. Get 16 bit out
uint16_t read_temp(uint32_t v){
	
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


int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	spi_init_module();
	//init_i2c();
	sei();            // enable global interrupts
	
	state = 1;
	
	while (1) {
		time1= rtc_get_time();
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		handle_spi_to_bbb();
		//ioport_set_pin_level(LED_0_PIN,LED_0_ACTIVE);
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			/*
			//Read from RHS
			uint16_t recieved_data;
			twi_package_t packet_read;
			memset(&packet_read, 0, sizeof(twi_package_t));
			packet_read.addr[0]			= 0x00;
			packet_read.addr_length		= sizeof(uint8_t);
			packet_read.chip			= 0x49;
			packet_read.buffer			= (uint8_t*)&recieved_data;
			packet_read.length			= sizeof(uint16_t);
			
			if(twi_master_read(&TWIC, &packet_read) == TWI_SUCCESS){
				sensor_data[4] = recieved_data;
				sensor_data[5] = recieved_data >> 8;
			}
			handle_spi_to_bbb();
			
			packet_read.chip			= 0x4A;
			if(twi_master_read(&TWIC, &packet_read) == TWI_SUCCESS){
				sensor_data[6] = recieved_data;
				sensor_data[7] = recieved_data >> 8;
			}
			handle_spi_to_bbb();
			
			packet_read.chip			= 0x4B;
			if(twi_master_read(&TWIC, &packet_read) == TWI_SUCCESS){
				sensor_data[8] = recieved_data;
				sensor_data[9] = recieved_data >> 8;
			}
			handle_spi_to_bbb();
			
			//Read from the Y,Z Values
			uint8_t setup_bytes_non[] = {0xc3, 0xe3};
			twi_package_t packet_write;
			memset(&packet_write,0,sizeof(twi_package_t));
			packet_write.addr[0]			= 0x01;
			packet_write.addr_length		= sizeof(uint8_t);
			packet_write.chip				= 0x48;
			packet_write.buffer				= (void *)(setup_bytes_non);
			packet_write.length				= sizeof(setup_bytes_non);
			
			//Start conversion to get Y
			if(twi_master_write(&TWIC, &packet_write) == TWI_SUCCESS){
				_delay_ms(1);
				handle_spi_to_bbb();
				if(twi_master_read(&TWIC, &packet_read) == TWI_SUCCESS){
					sensor_data[0] = recieved_data;
					sensor_data[1] = recieved_data >> 8;
				}
				handle_spi_to_bbb();
			}
			
			setup_bytes_non[0] = 0xd3; //get AIN1
			if(twi_master_write(&TWIC, &packet_write) == TWI_SUCCESS){
				_delay_ms(1);
				handle_spi_to_bbb();
				if(twi_master_read(&TWIC, &packet_read) == TWI_SUCCESS){
					sensor_data[2] = recieved_data;
					sensor_data[3] = recieved_data >> 8;
				}
				handle_spi_to_bbb();
			}
			
			//Read from SPI to get the Temperatures
			//Don't need to do this very Frequently. Temperatures don't change that fast
			//We should make it a second eventually. Simply don't need lots of temperature data
			if(time4 > APROX_HALF_SECOND ){
				time4 = 0;
				uint32_t data_buffer = 0;
				spi_select_device(&SPIF, &spi_pf0);
				spi_read_packet(&SPIF, (uint8_t *)&data_buffer, 4);
				spi_deselect_device(&SPIF, &spi_pf0);
				uint16_t value = read_temp(data_buffer);
				sensor_data[26] = value;
				sensor_data[27] = value >> 8;
				
				handle_spi_to_bbb();
			
				data_buffer = 0;
				spi_select_device(&SPIF, &spi_pf1);
				spi_read_packet(&SPIF, (uint8_t *)&data_buffer, 2);
				spi_deselect_device(&SPIF, &spi_pf1);
				value = read_temp(data_buffer);
				sensor_data[28] = value;
				sensor_data[29] = value >> 8;
				
				handle_spi_to_bbb();
			
				data_buffer = 0;
				spi_select_device(&SPIF, &spi_pf2);
				spi_read_packet(&SPIF, (uint8_t *)&data_buffer, 2);
				spi_deselect_device(&SPIF, &spi_pf2);
				value = read_temp(data_buffer);
				sensor_data[30] = value;
				sensor_data[31] = value >> 8;
				
				handle_spi_to_bbb();
			
				spi_select_device(&SPIF, &spi_pf3);
				spi_read_packet(&SPIF, (uint8_t *)&data_buffer, 2);
				spi_deselect_device(&SPIF, &spi_pf3);
				value = read_temp(data_buffer);
				sensor_data[32] = value;
				sensor_data[33] = value >> 8;
				
				handle_spi_to_bbb();
			
				spi_select_device(&SPIF, &spi_pr0);
				spi_read_packet(&SPIF, (uint8_t *)&data_buffer, 2);
				spi_deselect_device(&SPIF, &spi_pr0);
				value = read_temp(data_buffer);
				sensor_data[34] = value;
				sensor_data[35] = value >> 8;
				
			}
			
			//Get Batteries!
			if(time5 > APROX_HALF_SECOND/2){
				time5 = 0;
				
				setup_bytes_non[0]				= 0xc3;
				packet_write.chip				= 0x48;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[10] = recieved_data;
						sensor_data[11] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				
				setup_bytes_non[0]				= 0xd3;
				packet_write.chip				= 0x48;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[12] = recieved_data;
						sensor_data[13] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				
				setup_bytes_non[0]				= 0xc3;
				packet_write.chip				= 0x49;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[14] = recieved_data;
						sensor_data[15] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				setup_bytes_non[0]				= 0xd3;
				packet_write.chip				= 0x49;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[16] = recieved_data;
						sensor_data[17] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				
				setup_bytes_non[0]				= 0xc3;
				packet_write.chip				= 0x4A;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[18] = recieved_data;
						sensor_data[19] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				setup_bytes_non[0]				= 0xd3;
				packet_write.chip				= 0x4A;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[20] = recieved_data;
						sensor_data[21] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				
				setup_bytes_non[0]				= 0xc3;
				packet_write.chip				= 0x4B;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[22] = recieved_data;
						sensor_data[23] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				setup_bytes_non[0]				= 0xd3;
				packet_write.chip				= 0x4B;
				if(twi_master_write(&TWIE, &packet_write) == TWI_SUCCESS){
					_delay_ms(1);
					handle_spi_to_bbb();
					if(twi_master_read(&TWIE, &packet_read) == TWI_SUCCESS){
						sensor_data[24] = recieved_data;
						sensor_data[25] = recieved_data >> 8;
					}
					handle_spi_to_bbb();
				}
				
			}
			*/
		
		}
		time2 = rtc_get_time();
		time3 = time2-time1;
		time4 += time3;
		time5 += time3;
	}
	
}