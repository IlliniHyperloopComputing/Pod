
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
8,9,10,11 == Optical
*/

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

//RTC
uint32_t time1 = 0;
uint32_t time2 = 1;
uint32_t time3 = 2;

uint8_t retro_1_flag = 0;
uint8_t retro_2_flag = 0;
uint32_t retro_1_time = 0;
uint32_t retro_2_time = 0;
uint32_t retro_1_time_old = 0;
uint32_t retro_2_time_old = 0;
uint32_t delta_1 = UINT32_MAX;
uint32_t delta_2 = UINT32_MAX;

ISR(PORTK_INT0_vect){
	retro_1_time = rtc_get_time();
	delta_1 = retro_1_time - retro_1_time_old;
	retro_1_time_old = retro_1_time;
}
ISR(PORTF_INT0_vect){
	retro_2_time = rtc_get_time();
	delta_2 = retro_2_time - retro_2_time_old;
	retro_2_time_old = retro_2_time;
}

int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	ioport_configure_port_pin(&PORTK, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_SENSE_RISING | IOPORT_PULL_DOWN);
	PORTK.INT0MASK = PIN2_bm;
	PORTK.INTCTRL =	PORT_INT0LVL_MED_gc;
	
	ioport_configure_port_pin(&PORTF, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_SENSE_RISING | IOPORT_PULL_DOWN);
	PORTF.INT0MASK = PIN2_bm;
	PORTF.INTCTRL =	PORT_INT0LVL_MED_gc;
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	sensor_status = 0;
	sensor_status |= init_adc(&TWIF, 0x48, ADC_STREAMING) << 0;
	sensor_status |= init_adc(&TWIF, 0x49, ADC_STREAMING) << 1;
	sensor_status |= init_adc(&TWIF, 0x4A, ADC_STREAMING) << 2;
	sensor_status |= init_adc(&TWIF, 0x4B, ADC_STREAMING) << 3;	
		
	
	
	sei();            // enable global interrupts
		
	state = 1;
	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
	
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		handle_spi_to_bbb();
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			
			if(delta_1 < delta_2){
				sensor_data[8]  = delta_1 >> 0;
				sensor_data[9]  = delta_1 >> 8;
				sensor_data[10] = delta_1 >> 16;
				sensor_data[11] = delta_1 >> 24;
			}
			else{
				sensor_data[8]  = delta_2 >> 0;
				sensor_data[9]  = delta_2 >> 8;
				sensor_data[10] = delta_2 >> 16;
				sensor_data[11] = delta_2 >> 24;
			}	
			if(spi_isr) continue;
			
			
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
				
			
			time2 = rtc_get_time();
			time3 = time2-time1;
		
		}
	}
}
