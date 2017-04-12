
#include <asf.h>
#include <avr/interrupt.h>

//For memcpy
#include <string.h>

//Using crc-ccitt
#include <stdbool.h>
#include <assert.h>

#define SPI_TX_START 0xAA
#define SPI_CRC_PASS 0xAA
#define SPI_CRC_FAIL 0xFF

//Used in SPI ISR
volatile uint8_t rx_byte = 0x00;
volatile uint8_t spic_flag = 0;

//Incoming data
#define CMD_DATA_SIZE 5
uint8_t cmd_data[CMD_DATA_SIZE];
uint8_t cmd_idx = 0;
uint16_t received_crc = 0;
uint16_t calculated_crc = 0;

//Sensor data storage
uint8_t state = 0;
uint8_t sensor_status = 0;
#define SENSOR_DATA_SIZE 20
uint8_t sensor_data[SENSOR_DATA_SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

//Sending data
uint8_t cmd_finished = 0;
uint8_t send_data[SENSOR_DATA_SIZE+2];
uint8_t send_idx = 0;
uint8_t send_crc_length = 0;//set equal to maximum value of send_idx
uint16_t send_crc = 0;
uint8_t send_crc_idx = 0;



ISR(SPIC_INT_vect) {
	rx_byte = SPIC.DATA;
	spic_flag = 1;
}

void setUpSPIC()
{
	PORTC.DIR = 0x40;		// MISO output; MOSI, SCK, SS inputs
	SPIC.CTRL = 0x40;		// slave mode, mode 0
	SPIC.INTCTRL = 0x03;	// enable interrupts
}


int main (void)
{
	board_init();	//Init board
	//sysclk_init();	//Needed for CRC
	setUpSPIC();	//Setup SPI on Port C
	
	PMIC.CTRL = 0x04; // enable high priority interrupts
	sei();            // enable global interrupts
	
	
	while (1) {
		
		//SPIC handler
		//When this is true, it means we have just received a byte
		//So, we need to pipeline the next byte to be sent out.
		//Also, the most recently sent byte is in the rx_byte
		if(spic_flag){
			//Indicate start of incoming command
			if(rx_byte == SPI_TX_START){
				cmd_idx = CMD_DATA_SIZE;
				//Reset all the send variables/tmp storage
				cmd_finished = 0;
				send_idx = 0;
				send_crc_length = 0;
				send_crc = 0;
				send_crc_idx = 0;

			}
			
			//If we are receiving command, store it appropriately
			if(cmd_idx > 0){
				cmd_data[CMD_DATA_SIZE-cmd_idx] = rx_byte;
				cmd_idx--;
				
				//Finished last storage of incoming data
				if(cmd_idx == 0){
					//Check recieved_crc against calculated CRC
					received_crc =	(cmd_data[CMD_DATA_SIZE-1]<<8) | cmd_data[CMD_DATA_SIZE-2];
					calculated_crc = crc_io_checksum(cmd_data, CMD_DATA_SIZE-2, CRC_16BIT);
					//Send appropriate signal if passed/failed
					if(calculated_crc == received_crc){
						SPIC.DATA = SPI_CRC_PASS;
						cmd_finished = 1;
					}
					else{
						SPIC.DATA = SPI_CRC_FAIL;
					}
				}				
			}
			else if(cmd_finished){
				//On next pass we will be start pipelining data
				if(cmd_data[2] == 0){
					memcpy(send_data,sensor_data,SENSOR_DATA_SIZE);//TODO: determine if this takes too long
					send_idx = SENSOR_DATA_SIZE;
					send_crc_length = send_idx;
				}
				else if(cmd_data[2] == 1 || cmd_data[2] == 2){
					send_idx = 1;
					send_crc_length = send_idx;
				}
				else{
					memcpy(send_data,sensor_data,SENSOR_DATA_SIZE);
					send_data[SENSOR_DATA_SIZE] = state;
					send_data[SENSOR_DATA_SIZE+1] = sensor_status;
					send_idx = SENSOR_DATA_SIZE+2;
					send_crc_length = send_idx;
				}
				cmd_finished = 0;
			}
			
			
			if(send_idx > 0){
				
				if(cmd_data[2] == 0){//Send only sensor data
					SPIC.DATA = send_data[SENSOR_DATA_SIZE-send_idx];
				}
				else if(cmd_data[2] == 1){//send only state data
					SPIC.DATA = state;
				}
				else if(cmd_data[2] == 2){//send only sensor_status data
					SPIC.DATA = sensor_status;
				}
				else{//send all
					SPIC.DATA = send_data[SENSOR_DATA_SIZE+2-send_idx];
				}				
				send_idx--;
				
				//Calculate CRC
				if(send_idx == 0){
					send_crc = crc_io_checksum(send_data, send_crc_length, CRC_16BIT);
					send_crc_idx = 2;
				}
				
			}
			else if(send_crc_idx > 0){
				SPIC.DATA = send_crc >> ((2-send_crc_idx)*8);
				send_crc_idx--;
			}
			
			spic_flag = 0;
		}
	}
}