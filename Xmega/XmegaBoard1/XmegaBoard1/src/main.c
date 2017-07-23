
#include <asf.h>
#include <avr/io.h>
#include <stdbool.h>
#include <assert.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>

#include "spi_to_bbb.h"
#include "i2c_sensors.h"


//Sensor data storage
//spi_to_bbb externs these variables
uint8_t state = 0;
uint8_t sensor_status = 0;
uint8_t sensor_data[SENSOR_DATA_SIZE] = {0};
/*
0,1 == X0
2,3 == X1
4,5 == X2
6,7 == Brake
8,9 == Optical
*/

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

//RTC
uint32_t time1 = 0;
uint32_t time2 = 1;
uint32_t time3 = 2;

int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	sensor_status = 0;
	sensor_status |= init_adc(&TWIF, 0x48, ADC_STREAMING) << 0;
	sensor_status |= init_adc(&TWIF, 0x49, ADC_STREAMING) << 1;
	sensor_status |= init_adc(&TWIF, 0x4A, ADC_STREAMING) << 2;
	sensor_status |= init_adc(&TWIF, 0x4B, ADC_STREAMING) << 3;	
		
	
	sei();            // enable global interrupts
		
	state = 1;
	
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		handle_spi_to_bbb();
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			time1= rtc_get_time();
			
			uint8_t recieved_data[2] = {0};

			if(read_adc(&TWIF, 0x48, recieved_data ) == TWI_SUCCESS){
				sensor_data[0] = recieved_data[1];
				sensor_data[1] = recieved_data[0];
			}
			
			if(read_adc(&TWIF, 0x49, recieved_data ) == TWI_SUCCESS){
				sensor_data[2] = recieved_data[1];
				sensor_data[3] = recieved_data[0];
			}
			
			if(read_adc(&TWIF, 0x4A, recieved_data ) == TWI_SUCCESS){
				sensor_data[4] = recieved_data[1];
				sensor_data[5] = recieved_data[0];
			}
			
			if(read_adc(&TWIF, 0x4B, recieved_data ) == TWI_SUCCESS){
				sensor_data[6] = recieved_data[1];
				sensor_data[7] = recieved_data[0];
			}
			if(spi_isr) continue;
			//handle_spi_to_bbb();
				
			
			time2 = rtc_get_time();
			time3 = time2-time1;
			sensor_data[8] = time3 >> 0;
			sensor_data[9] = time3 >> 8;
		
		}
	}
}
