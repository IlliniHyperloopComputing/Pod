
#include <asf.h>
#include <avr/io.h>
#include <stdbool.h>
#include <assert.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>

#include "spi_to_bbb.h"
#include "i2c_sensors.h"

#define APROX_HALF_SECOND  (16328)

#define NO_CMD 0
#define STATE_UNINIT  1
#define STATE_COLLECT 2
#define STATE_MANUAL_BRAKE 3
#define STATE_PID_BRAKE 4
#define CMD_RESET 5
#define CMD_INIT_DEADMAN 6
#define STATE_MANUAL_REVERSE_BRAKE 7
#define STATE_MANUAL_REVERSE_BRAKE_NEXT 8

#define ACCEL_MIN (4800) //-3 g
#define ACCEL_MAX (21000) //3 g

#define ACCEL_SENSOR_YZ (0x48)
#define ACCEL_SENSOR_1  (0x48)
#define ACCEL_SENSOR_2  (0x49)
#define ACCEL_SENSOR_3  (0x4a)

//Sensor data storage
//spi_to_bbb externs these variables
uint8_t state = 0;
uint8_t sensor_status = 0;
uint8_t sensor_data[SENSOR_DATA_SIZE] = {0};
uint8_t recv_cmd = 0;
/*
0,1 == X0
2,3 == X1
4,5 == X2
6,7 == Y
8,9 == Z
*/

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	
	sensor_status = 0;
	init_adc(&TWIC, ACCEL_SENSOR_YZ, ADC_SINGLE);

	init_adc(&TWIF, ACCEL_SENSOR_1, ADC_STREAMING);
	init_adc(&TWIF, ACCEL_SENSOR_2, ADC_STREAMING);
	init_adc(&TWIF, ACCEL_SENSOR_3, ADC_STREAMING);
	
	sei();            // enable global interrupts
		
	state = STATE_COLLECT;
	
	//Used to indicate that we did not hang in the setup stage
	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
	
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		handle_spi_to_bbb();
		
		if(spi_transfer == 0){//Do anything that is not SPI related
			
			if(recv_cmd == CMD_RESET){
				//Need to reset any of the sensors that accumulate data / have history
				//Specifically this is just the optical tape count
			}
			recv_cmd = 0;
			
			if(spi_isr) continue;
						
			//Read in Sensors
			uint8_t recieved_data[2] = {0};

			set_adc_mux(&TWIC, ACCEL_SENSOR_YZ, AIN0);//Set to read Y value

			//read x accel1
			if(read_adc(&TWIF, ACCEL_SENSOR_1, recieved_data ) == TWI_SUCCESS){
				sensor_data[0] = recieved_data[1];
				sensor_data[1] = recieved_data[0];
			}

			//read y accel
			if(read_adc(&TWIC, ACCEL_SENSOR_YZ, recieved_data) == TWI_SUCCESS){
				sensor_data[6] = recieved_data[1];
				sensor_data[7] = recieved_data[0];
			}
			
			if(spi_isr) continue;
			
			set_adc_mux(&TWIC, ACCEL_SENSOR_YZ, AIN1);//Set to read Z value
			
			//read x accel2
			if(read_adc(&TWIF, ACCEL_SENSOR_2, recieved_data ) == TWI_SUCCESS){
				sensor_data[2] = recieved_data[1];
				sensor_data[3] = recieved_data[0];
			}

			//read z accel
			if(read_adc(&TWIC, ACCEL_SENSOR_YZ, recieved_data) == TWI_SUCCESS){
				sensor_data[8] = recieved_data[1];
				sensor_data[9] = recieved_data[0];

			}

			if(spi_isr) continue;
			
			//read x accel3
			if(read_adc(&TWIF, ACCEL_SENSOR_3, recieved_data ) == TWI_SUCCESS){
				sensor_data[4] = recieved_data[1];
				sensor_data[5] = recieved_data[0];
			}
		}
	}
}
