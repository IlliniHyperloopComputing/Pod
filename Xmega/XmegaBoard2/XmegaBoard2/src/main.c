
#include <asf.h>
#include <avr/io.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>
#define APROX_HALF_SECOND 151000

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
24    == RetroReflective  Interrupt
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


int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	/*
	init_adc(&TWIC, 0x48, ADC_SINGLE);//Read Y, Z accel
	init_adc(&TWIE, 0x48, ADC_SINGLE);//Read Battery Voltages
	init_adc(&TWIC, 0x49, ADC_STREAMING);//Read RHS
	init_adc(&TWIE, 0x49, ADC_SINGLE);//Read Battery Voltages
	init_adc(&TWIC, 0x4a, ADC_STREAMING);//Read RHS
	init_adc(&TWIE, 0x4a, ADC_SINGLE);//Read Battery Voltages
	init_adc(&TWIC, 0x4b, ADC_STREAMING);//Read RHS
	init_adc(&TWIE, 0x4b, ADC_SINGLE);//Read Battery Voltages

	init_thermo_sensors();
	*/
	
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
			uint16_t recieved_data = 0;
			
			if(read_adc(&TWIC, 0x49, &recieved_data) == TWI_SUCCESS){
				sensor_data[4] = recieved_data;
				sensor_data[5] = recieved_data >> 8;
			}
			
			if(read_adc(&TWIC, 0x4a, &recieved_data) == TWI_SUCCESS){
				sensor_data[6] = recieved_data;
				sensor_data[7] = recieved_data >> 8;
			}
			
			if(read_adc(&TWIC, 0x4b, &recieved_data) == TWI_SUCCESS){
				sensor_data[8] = recieved_data;
				sensor_data[9] = recieved_data >> 8;
			}

			handle_spi_to_bbb();
			
			//Read from the Y,Z Values
			set_adc_mux(&TWIC, 0x48, AIN0);
			_delay_ms(1);
			if(read_adc(&TWIC, 0x48, &recieved_data) == TWI_SUCCESS){
				sensor_data[0] = recieved_data;
				sensor_data[1] = recieved_data >> 8;
			}
			
			set_adc_mux(&TWIC, 0x48, AIN1);
			_delay_ms(1);
			if(read_adc(&TWIC, 0x48, &recieved_data) == TWI_SUCCESS){
				sensor_data[2] = recieved_data;
				sensor_data[3] = recieved_data >> 8;
			}
			
			//Read from SPI to get the Temperatures
			//Don't need to do this very Frequently. Temperatures don't change that fast
			//We should make it a second eventually. Simply don't need lots of temperature data
			if(time4 > APROX_HALF_SECOND ){
				time4 = 0;
				uint16_t value;

				value = thermo_external_temp(read_thermo(0));
				sensor_data[14] = value;
				sensor_data[15] = value >> 8;
				
				value = thermo_external_temp(read_thermo(1));
				sensor_data[16] = value;
				sensor_data[17] = value >> 8;
				
				value = thermo_external_temp(read_thermo(2));
				sensor_data[18] = value;
				sensor_data[19] = value >> 8;
				
				uint32_t rt = read_thermo(3);
				value = thermo_external_temp(rt);
				sensor_data[20] = value;
				sensor_data[21] = value >> 8;

				value = thermo_internal_temp(rt);
				sensor_data[22] = value;
				sensor_data[23] = value >> 8;

				
			}
			
			//Get Batteries!
			if(time5 > APROX_HALF_SECOND/2){
				time5 = 0;
				
				set_adc_mux(&TWIE, 0x48, AIN0);
				_delay_ms(1);
				if(read_adc(&TWIE, 0x48, &recieved_data) == TWI_SUCCESS){
					sensor_data[10] = recieved_data;
					sensor_data[11] = recieved_data >> 8;
				}

				set_adc_mux(&TWIE, 0x48, AIN1);
				_delay_ms(1);
				if(read_adc(&TWIE, 0x48, &recieved_data) == TWI_SUCCESS){
					sensor_data[12] = recieved_data;
					sensor_data[13] = recieved_data >> 8;
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
