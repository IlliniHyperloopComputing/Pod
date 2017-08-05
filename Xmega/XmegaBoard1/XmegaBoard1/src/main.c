
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

#define PORT_SETUP_WITH_INVERT  (PORT_OPC_PULLDOWN_gc | PORT_INVEN_bm | PORT_ISC_LEVEL_gc)
#define PORT_SETUP_WITHOUT_INVERT (PORT_OPC_PULLUP_gc | PORT_ISC_LEVEL_gc)

uint8_t is_invert_1 = 1;
uint8_t is_invert_2 = 1;
volatile uint8_t flip = 1;

void handle_optical(){
	uint8_t val_1 = (PORTK.IN & PIN2_bm) >> PIN2_bp;

	if(!high_1){
		if(val_1){
			cooldown_1++;
			if(cooldown_1 > COOLDOWN){
				retro_1_time = rtc_get_time();
				delta_1 = retro_1_time - retro_1_time_old;
				retro_1_time_old = retro_1_time;
				rotation_count_1++;
				high_1 = 1;
				cooldown_1 =0;
			}
		}
		else{
			cooldown_1 =0;
		}
	}
	else{
		if(!val_1){
			cooldown_1++;
			if(cooldown_1 > COOLDOWN){
				high_1 = 0;
				cooldown_1 = 0;
			}
		}
		else{
			cooldown_1 =0;
		}
	}

	uint8_t val_2 = (PORTF.IN & PIN2_bm) >> PIN2_bp;
	
	if(!high_2){
		if(val_2){
			cooldown_2++;
			if(cooldown_2 > COOLDOWN){
				retro_2_time = rtc_get_time();
				delta_2 = retro_2_time - retro_2_time_old;
				retro_2_time_old = retro_2_time;
				rotation_count_1++;
				high_2 = 1;
				cooldown_2 =0;
			}
		}
		else{
			cooldown_2 =0;
		}
	}
	else{
		if(!val_2){
			cooldown_2++;
			if(cooldown_2 > COOLDOWN){
				high_2 = 0;
				cooldown_2 = 0;
			}
		}
		else{
			cooldown_2 =0;
		}
	}
}

int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	ioport_configure_port_pin(&PORTK, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN );
	
	/*PORTK.DIR = 0;//pin 2 is output
	PORTK.INT0MASK = PIN3_bm;
	PORTK.INTCTRL = PORT_INT0LVL_MED_gc;
	PORTK.PIN3CTRL = PORT_OPC_PULLDOWN_gc | PORT_ISC_RISING_gc;
	*/
	
	

	ioport_configure_port_pin(&PORTF, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN );
	/*PORTF.DIR = 0;
	PORTF.INT0MASK = PIN2_bm;
	PORTF.INTCTRL = PORT_INT0LVL_MED_gc;
	PORTF.PIN2CTRL =  PORT_SETUP_WITH_INVERT;
	*/
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, handle_optical);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 300);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1_gc);
	
	sensor_status = 0;
	sensor_status |= init_adc(&TWIF, 0x48, ADC_STREAMING) << 0;
	sensor_status |= init_adc(&TWIF, 0x49, ADC_STREAMING) << 1;
	sensor_status |= init_adc(&TWIF, 0x4A, ADC_STREAMING) << 2;
	sensor_status |= init_adc(&TWIF, 0x4B, ADC_STREAMING) << 3;	
	
	sei();            // enable global interrupts
		
	state = 1;
	//ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
	
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		handle_spi_to_bbb();
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			
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
			if(spi_isr) continue;
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
			
			if(spi_isr) continue;
			
			if(read_adc(&TWIF, 0x4A, recieved_data ) == TWI_SUCCESS){
				sensor_data[4] = recieved_data[1];
				sensor_data[5] = recieved_data[0];
			}
			
			if(read_adc(&TWIF, 0x4B, recieved_data ) == TWI_SUCCESS){
				sensor_data[6] = recieved_data[1];
				sensor_data[7] = recieved_data[0];
			}
			
		
		}
	}
}
