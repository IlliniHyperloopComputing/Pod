
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
#define APROX_HALF_SECOND  (16328)

#define STATE_COLLECT 1
#define STATE_MANUAL_BRAKE 2

#define ACCEL_MIN (1000)
#define ACCEL_MAX (10000)

#define BRAKE_MIN (1000)
#define BRAKE_MAX (10000)

#define ACCEL_SENSOR_1  (0x48)
#define ACCEL_SENSOR_2  (0x49)
#define ACCEL_SENSOR_3  (0x4a)
#define BRAKE_SENSOR_1  (0x4b)

//Sensor data storage
//spi_to_bbb externs these variables
uint8_t state = 0;
uint8_t sensor_status = 0;
uint8_t sensor_data[SENSOR_DATA_SIZE] = {0};
uint8_t recv_cmd = 0;
uint16_t brake_pressure = 0;
uint16_t accel_1 = 0;
uint16_t accel_2 = 0;
uint16_t accel_3 = 0;

#define SET_BRAKE_LOW()   PORTK.OUT &= !(PIN3_bm)
#define SET_BRAKE_HIGH()  PORTK.OUT |= (PIN3_bm)


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
uint16_t true_accel = 0;

uint32_t brake_manual_start = 0;


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
	ioport_configure_port_pin(&PORTF, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN );
	ioport_configure_port_pin(&PORTK, PIN3_bm, IOPORT_DIR_OUTPUT | IOPORT_TOTEM | IOPORT_INIT_LOW );
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	tc_enable(&TCC0);
	tc_set_overflow_interrupt_callback(&TCC0, handle_optical);
	tc_set_wgm(&TCC0, TC_WG_NORMAL);
	tc_write_period(&TCC0, 300);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1_gc);
	
	sensor_status = 0;
	sensor_status |= init_adc(&TWIF, ACCEL_SENSOR_1, ADC_STREAMING) << 0;
	sensor_status |= init_adc(&TWIF, ACCEL_SENSOR_2, ADC_STREAMING) << 1;
	sensor_status |= init_adc(&TWIF, ACCEL_SENSOR_3, ADC_STREAMING) << 2;
	sensor_status |= init_adc(&TWIF, BRAKE_SENSOR_1, ADC_STREAMING) << 3;	
	
	sei();            // enable global interrupts
		
	state = STATE_COLLECT;
	
	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		handle_spi_to_bbb();
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			//Sensor sanity check
			sensor_status |= (accel_1 < ACCEL_MIN || accel_1 > ACCEL_MAX) << 0;
			sensor_status |= (accel_2 < ACCEL_MIN || accel_2 > ACCEL_MAX) << 1;
			sensor_status |= (accel_3 < ACCEL_MIN || accel_3 > ACCEL_MAX) << 2;
			sensor_status |= (brake_pressure < BRAKE_MIN || brake_pressure > BRAKE_MAX) << 3;
			
			//Select median accelerometer
			if(accel_1 > accel_2){
				if(accel_2 > accel_3){
					true_accel = accel_2;
				}
				else if(accel_1 > accel_3){
					true_accel = accel_3;
				}
				else{
					true_accel = accel_1;
				}
			}
			else{
				if(accel_1 > accel_3){
					true_accel = accel_1;
				}
				else if(accel_2 > accel_3){
					true_accel = accel_3
				}
				else{
					true_accel = accel_2;
				}
			}
			
			//State Machine
			if(state == STATE_COLLECT){
				//Turn on Manual Brake
				if(recv_cmd == STATE_MANUAL_BRAKE){
					brake_manual_start = rtc_get_time();
					SET_BRAKE_HIGH();
					state = recv_cmd;
				}
				recv_cmd = 0;
			}
			else if(state == STATE_MANUAL_BRAKE){
				//Switch to 
				if(recv_cmd == STATE_COLLECT){
					brake_manual_start = 0;//set this to zero to trigger the following if()
					state = STATE_COLLECT;
				}
				
				if(rtc_get_time() - brake_manual_start >= APROX_HALF_SECOND ){
					SET_BRAKE_LOW();
					state = STATE_COLLECT;
				}
				recv_cmd = 0;
			}
			
			//Select correct speed data
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
						
			//Read in Sensors
			uint8_t recieved_data[2] = {0};

			if(read_adc(&TWIF, ACCEL_SENSOR_1, recieved_data ) == TWI_SUCCESS){
				sensor_data[0] = recieved_data[1];
				sensor_data[1] = recieved_data[0];
				accel_1 |= recieved_data[1];
				accel_1 |= recieved_data[0] << 8;
			}
			
			if(read_adc(&TWIF, ACCEL_SENSOR_2, recieved_data ) == TWI_SUCCESS){
				sensor_data[2] = recieved_data[1];
				sensor_data[3] = recieved_data[0];
				accel_2 |= recieved_data[1];
				accel_2 |= recieved_data[0] << 8;
			}
			
			if(spi_isr) continue;
			
			if(read_adc(&TWIF, ACCEL_SENSOR_3, recieved_data ) == TWI_SUCCESS){
				sensor_data[4] = recieved_data[1];
				sensor_data[5] = recieved_data[0];
				accel_3 |= recieved_data[1];
				accel_3 |= recieved_data[0] << 8;
			}
			
			if(read_adc(&TWIF, BRAKE_SENSOR_1, recieved_data ) == TWI_SUCCESS){
				sensor_data[6] = recieved_data[1];
				sensor_data[7] = recieved_data[0];
				brake_pressure |= recieved_data[1];
				brake_pressure |= recieved_data[0] << 8;
				
			}
			
		
		}
	}
}
