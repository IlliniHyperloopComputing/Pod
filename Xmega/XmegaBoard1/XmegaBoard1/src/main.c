
#include <asf.h>
#include <avr/io.h>
#include <stdbool.h>
#include <assert.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>

#include "spi_to_bbb.h"
#include "i2c_sensors.h"

#define COOLDOWN 2


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
8,9,10,11   == Optical. Delta between strips
12,13,14,15 == Optical. Rotation count

*/

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

//RTC
uint32_t time1 = 0;
uint32_t time2 = 1;
uint32_t time3 = 2;

volatile uint32_t retro_1_time = 0;
volatile uint32_t retro_2_time = 0;
volatile uint32_t retro_1_time_old = 0;
volatile uint32_t retro_2_time_old = 0;
volatile uint32_t delta_1 = UINT32_MAX;
volatile uint32_t delta_2 = UINT32_MAX;
volatile uint32_t rotation_count_1 = 0;
volatile uint32_t rotation_count_2 = 0;

uint8_t high_1 = 1;
uint8_t high_2 = 1;
uint8_t cooldown_1 = 0;
uint8_t cooldown_2 = 0;

uint32_t true_delta = UINT32_MAX;
uint32_t true_rotation_count = 0;

int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	ioport_configure_port_pin(&PORTK, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);

	ioport_configure_port_pin(&PORTF, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	
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
			
			uint8_t val_1 = (PORTK.IN & PIN2_bm) >> PIN2_bp;
			
			if(val_1 && !high_1 ){
				retro_1_time = rtc_get_time();
				delta_1 = retro_1_time - retro_1_time_old;
				retro_1_time_old = retro_1_time;
				rotation_count_1++;
				high_1 = 1;
			}
			if(!val_1 && high_1){
				cooldown_1++;
				if(cooldown_1 > COOLDOWN){
					high_1 = 0;
					cooldown_1 = 0;
				}
			}
			if(spi_isr) continue;
			
			uint8_t val_2 = (PORTF.IN & PIN2_bm) >> PIN2_bp;
			
			if(val_2 && !high_2 ){
				retro_2_time = rtc_get_time();
				delta_2 = retro_2_time - retro_2_time_old;
				retro_2_time_old = retro_2_time;
				rotation_count_2++;
				high_2 = 1;
			}
			if(!val_2 && high_2){
				cooldown_2++;
				if(cooldown_2 > COOLDOWN){
					high_2 = 0;
					cooldown_2 = 0;
				}
			}
			
			if(spi_isr) continue;
			
			if(delta_1 < delta_2){
				true_delta = delta_1;
			}
			else{
				true_delta = delta_2;
			}				
			if(rotation_count_1 > rotation_count_2){
				true_rotation_count = rotation_count_1;
			}
			else{
				true_rotation_count = rotation_count_2;
			}
			memcpy(sensor_data + 8, (char *)&true_delta, 4);
			memcpy(sensor_data + 12, (char *)&true_rotation_count, 4);
			
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
