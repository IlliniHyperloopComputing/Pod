
#include <asf.h>

//for _delay_ms
#define F_CPU 32000000
#include <util/delay.h>
const uint32_t APROX_HALF_SECOND = 16328;
const uint32_t APROX_50_MILLI = 1639;

#include "spi_to_bbb.h"
#include "optical.h"

//Using crc-ccitt
#include <stdbool.h>
#include <assert.h>

//Sensor data storage
//spi_to_bbb externs these variables
uint8_t state = 0;
uint8_t sensor_status = 0;
uint8_t sensor_data[SENSOR_DATA_SIZE] = {0};
uint8_t recv_cmd = 0;
/*
index to value
0,1,2,3 = Tach1. time since last
4,5,6,7 = Tach2. time since last
8,9,10,11 = Tach3. time since last
12,13,14,15 = Tach4. time since last
16,17,18,19 = Optical encoder. time since last
17,18   = Tach1. delta
19,20   = Tach2. delta
21,22   = Tach3. delta
23,24   = Tach4. delta
25,26   = Optical encoder, delta
27,28   = Optical encoder, strip counts 

*/

#define NO_CMD 0
#define STATE_UNINIT  1
#define STATE_COLLECT 2
#define STATE_MANUAL_BRAKE 3
#define STATE_PID_BRAKE 4
#define CMD_RESET 5
#define CMD_INIT_DEADMAN 6
#define STATE_MANUAL_REVERSE_BRAKE 7

//lock
volatile uint8_t spi_isr = 0;
uint8_t spi_transfer = 0;

uint32_t time_since[5] = {0};
uint32_t times[5] = {0};
uint32_t old_times[5] = {0};
uint16_t deltas[5] = {0};
uint8_t high[5] = {0};
uint8_t cooldown[5] = {0};

uint16_t optical_strip_count = 0;

int main (void)
{
	board_init();	//Init board
	sysclk_init();
	rtc_init();	
	init_spi_to_bbb();	//Setup SPI on Port C
	
	ioport_configure_port_pin(&PORTE, PIN6_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);//EXT1-5
	ioport_configure_port_pin(&PORTE, PIN7_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);//EXT1-6
	ioport_configure_port_pin(&PORTE, PIN1_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);//EXT1-7
	ioport_configure_port_pin(&PORTE, PIN2_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);//EXT1-8
	ioport_configure_port_pin(&PORTR, PIN0_bm, IOPORT_DIR_INPUT | IOPORT_PULL_DOWN);//EXT1-9
	
	PMIC.CTRL |= PMIC_MEDLVLEN_bm;
	PMIC.CTRL |= PMIC_LOLVLEN_bm;	
	sei();            // enable global interrupts
	
	ioport_set_pin_level(LED_0_PIN,LED_0_ACTIVE);
	
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
				optical_strip_count = 0;
			}
			recv_cmd = 0;
			
			//Read in all of the tach sensors
			uint8_t val = read_val(PORTE.IN, PIN6_bm, PIN6_bp);
			debounce(val,high[0], cooldown[0], times[0], old_times[0], *(uint16_t*)(sensor_data+17) )
			val = read_val(PORTE.IN, PIN7_bm, PIN7_bp);
			debounce(val,high[1], cooldown[1], times[1], old_times[1], *(uint16_t*)(sensor_data+19) );
			val = read_val(PORTE.IN, PIN1_bm, PIN1_bp);
			debounce(val,high[2], cooldown[2], times[2], old_times[2], *(uint16_t*)(sensor_data+21) );
			val = read_val(PORTE.IN, PIN2_bm, PIN2_bp);
			debounce(val,high[3], cooldown[3], times[3], old_times[3], *(uint16_t*)(sensor_data+23) );
			
			//read in the optical encoder
			val = read_val(PORTR.IN, PIN0_bm, PIN0_bp);
			debounce_count(val,  (*(uint16_t*)(sensor_data+27)) , high[4], cooldown[4], times[4], old_times[4], *(uint16_t*)(sensor_data+25) )
			
			uint32_t now = rtc_get_time();
			*(uint32_t*)(sensor_data + 0) = now - times[0];
			*(uint32_t*)(sensor_data + 4) = now - times[1];
			*(uint32_t*)(sensor_data + 8) = now - times[2];
			*(uint32_t*)(sensor_data + 12) = now - times[3];
			*(uint32_t*)(sensor_data + 16) = now - times[4];
			
		}
	}
	
}
