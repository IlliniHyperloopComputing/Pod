
#include <asf.h>
#include <avr/io.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>
const uint32_t APROX_HALF_SECOND = 16328;
const uint32_t APROX_50_MILLI = 1639;

#define COOLDOWN 2

#include "spi_to_bbb.h"
#include "i2c_sensors.h"
#include "spi_sensors.h"

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
12,13 == Battery i2c
14,15 == Thermo0 Spi
16,17 == Thermo1
18,19 == Thermo2
20,21 == Thermo3 external
22,23 == Thermo3 internal
24,25 == Current to PCB
26    == RetroReflective  Interrupt
*/

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

//RTC
uint32_t time1 = 0;
uint32_t time2 = 1;
uint32_t time3 = 2;
uint32_t time4 = 1000;//offset to time 5
uint32_t time5 = 0;

uint8_t retro_1_flag = 0;
uint8_t retro_2_flag = 0;
uint8_t retro_3_flag = 0;
uint32_t retro_1_time = 0;
uint32_t retro_2_time = 0;
uint32_t retro_3_time = 0;


uint8_t high_1 = 1;
uint8_t high_2 = 1;
uint8_t high_3 = 1;
uint8_t cooldown_1 = 0;
uint8_t cooldown_2 = 0;
uint8_t cooldown_3 = 0;

int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	ioport_configure_port_pin(&PORTE, PIN6_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	ioport_configure_port_pin(&PORTE, PIN7_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	ioport_configure_port_pin(&PORTF, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	init_adc(&TWIC, 0x48, ADC_SINGLE);//Read Y, Z accel
	init_adc(&TWIE, 0x48, ADC_SINGLE);//Read Battery Voltages
	init_adc(&TWIC, 0x49, ADC_STREAMING);//Read RHS
	init_adc(&TWIC, 0x4a, ADC_STREAMING);//Read RHS
	init_adc(&TWIC, 0x4b, ADC_STREAMING);//Read RHS
	
	init_current(&TWIE, 0x40);

	init_thermo_sensors();
	
	sei();            // enable global interrupts
	
	state = 1;
	//ioport_set_pin_level(LED_0_PIN,LED_0_ACTIVE);
	
	while (1) {
		
		time1= rtc_get_time();
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		handle_spi_to_bbb();
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			uint8_t val_1 = (PORTE.IN & PIN6_bm) >> PIN6_bp;
			
			if(val_1 && !high_1 ){
				retro_1_flag = 1;
				retro_1_time = rtc_get_time();
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
			
			uint8_t val_2 = (PORTE.IN & PIN7_bm) >> PIN7_bp;
			
			if(val_2 && !high_2 ){
				retro_2_flag = 1;
				retro_2_time = rtc_get_time();
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
			
			uint8_t val_3 = (PORTF.IN & PIN2_bm) >> PIN2_bp;
			
			if(val_3 && !high_3 ){
				retro_3_flag = 1;
				retro_3_time = rtc_get_time();
				high_3 = 1;
			}
			
			if(!val_3 && high_3){
				cooldown_3++;
				if(cooldown_3 > COOLDOWN){
					high_3 = 0;
					cooldown_3 = 0;
				}
			}
			
			if(spi_isr) continue;
			
			//Checks if any 2 flags are true
			uint8_t retro_flag = (retro_1_flag && (retro_2_flag || retro_3_flag)) || (retro_2_flag && retro_3_flag);
			if(retro_flag){
				//idea is to see if any two of the times are within 50 mill of eachother.
				//this math will calculate if the numbers subtracted (which could overflow since uints), plus 50 milliseconds is less than 100 milliseconds. 
				//the values, if initially overflowed, should wrap back with the addition of 50 milliseconds if a valid time
				//the maximum value is less than 100 milliseconds. So just check if under 100 milliseconds.
				uint8_t f1 = (retro_1_time - retro_2_time + APROX_50_MILLI) < (2 * APROX_50_MILLI);
				uint8_t f2 = (retro_1_time - retro_3_time + APROX_50_MILLI) < (2 * APROX_50_MILLI);
				uint8_t f3 = (retro_2_time - retro_3_time + APROX_50_MILLI) < (2 * APROX_50_MILLI);
				
				//if any of the flags are true, proceed
				retro_flag = f1 || f2 || f3;
				if(retro_flag){
					sensor_data[26] ++;
				}
				
				retro_1_flag = 0;
				retro_2_flag = 0;
				retro_3_flag = 0;
				retro_1_time = 0;
				retro_2_time = 0;
				retro_3_time = 0;
			}
			
			
			//Read from RHS
			uint16_t recieved_data = 0;
			
			set_adc_mux(&TWIC, 0x48, AIN0);//Set to read from Y value. Offset for timing
			_delay_ms(0.5);
			if(read_adc(&TWIC, 0x49, &recieved_data) == TWI_SUCCESS){
				sensor_data[4] = recieved_data >> 8;
				sensor_data[5] = recieved_data;
			}
			
			//Read the Y value
			if(read_adc(&TWIC, 0x48, &recieved_data) == TWI_SUCCESS){
				sensor_data[0] = recieved_data >> 8;
				sensor_data[1] = recieved_data;
			}
			
			if(spi_isr) continue;	
			
			
			//Set to read Z value
			set_adc_mux(&TWIC, 0x48, AIN1);
			
			if(read_adc(&TWIC, 0x4a, &recieved_data) == TWI_SUCCESS){
				sensor_data[6] = recieved_data >> 8;
				sensor_data[7] = recieved_data;
			}
			
			if(read_adc(&TWIC, 0x4b, &recieved_data) == TWI_SUCCESS){
				sensor_data[8] = recieved_data >> 8;
				sensor_data[9] = recieved_data;
			}
			
			//Read Z value
			if(read_adc(&TWIC, 0x48, &recieved_data) == TWI_SUCCESS){
				sensor_data[2] = recieved_data >> 8;
				sensor_data[3] = recieved_data;
			}

			if(spi_isr) continue;			
			
			//Read from SPI to get the Temperatures
			//Don't need to do this very Frequently. Temperatures don't change that fast
			//We should make it a second eventually. Simply don't need lots of temperature data
			if(time4 > APROX_HALF_SECOND*4 ){
				time4 = 0;
				uint16_t value;

				value = thermo_external_temp(read_thermo(0));
				sensor_data[14] = value >> 0;
				sensor_data[15] = value >> 8;
				
				value = thermo_external_temp(read_thermo(1));
				sensor_data[16] = value >> 0;
				sensor_data[17] = value >> 8;
				
				value = thermo_external_temp(read_thermo(2));
				sensor_data[18] = value >> 0;
				sensor_data[19] = value >> 8;
				
				uint32_t rt = read_thermo(3);
				value = thermo_external_temp(rt);
				sensor_data[20] = value >> 0;
				sensor_data[21] = value >> 8;

				value = thermo_internal_temp(rt);
				sensor_data[22] = value >> 0;
				sensor_data[23] = value >> 8;

				
			}
			
			//Get Batteries and Current!
			if(time5 > APROX_HALF_SECOND*2){
				time5 = 0;
				
				set_adc_mux(&TWIE, 0x48, AIN0);
				
				//Read current
				if(read_current(&TWIE, 0x40, &recieved_data) == TWI_SUCCESS){
					sensor_data[24] = recieved_data >> 8;
					sensor_data[25] = recieved_data;
					ioport_set_pin_level(LED_0_PIN,LED_0_ACTIVE);
				}
				
				if(read_adc(&TWIE, 0x48, &recieved_data) == TWI_SUCCESS){
					sensor_data[10] = recieved_data >> 8;
					sensor_data[11] = recieved_data;
				}

				set_adc_mux(&TWIE, 0x48, AIN1);
				_delay_ms(1);
				if(read_adc(&TWIE, 0x48, &recieved_data) == TWI_SUCCESS){
					sensor_data[12] = recieved_data >> 8;
					sensor_data[13] = recieved_data;
				}
				
			}
			
			
		}
		time2 = rtc_get_time();
		time3 = time2-time1;
		time4 += time3;
		time5 += time3;
	}
	
}
