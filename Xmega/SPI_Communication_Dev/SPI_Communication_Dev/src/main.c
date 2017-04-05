
#include <asf.h>
#include <avr/interrupt.h>

//Using crc-ccitt
#include <stdbool.h>
#include <assert.h>

#define SPI_TX_START 0xAA

//Used in SPI ISR
volatile uint8_t rx_byte = 0x00;
volatile uint8_t spic_flag = 0;

//Incoming data
#define CMD_DATA_SIZE 5
uint8_t cmd_data[CMD_DATA_SIZE];
uint8_t cmd_idx = 0;
uint16_t received_crc = 0;
uint16_t calculated_crc = 0;

//Sending data


//Sensor data storage
uint8_t state = 0;
uint8_t sensor_status = 0;
#define SENSOR_DATA_SIZE 20
uint8_t sensor_data[SENSOR_DATA_SIZE];

volatile uint8_t tx_byte = 0xCC;
volatile uint8_t isr_flag = 0;
volatile uint8_t  i = 0;
uint16_t checksum = 0xFFFF;
uint8_t rx_buff[12] = {0xAB,0xB,0xC,0xF,0xE,0x55,0x66,0x77,0x88,0x99,0xbb,0xaa};
uint8_t sent_buff[12];
	
uint8_t data_len = 12;
uint8_t data_idx = 0;
uint8_t start_tx = 0;

ISR(SPIC_INT_vect) {
	//isr_flag = !isr_flag;
	//ioport_set_pin_level(LED_0_PIN,!isr_flag );
	//while(!(SPIC.STATUS & 0x80));
	//SPIC.DATA = 0xEF;     // send back to the master
	//isr_flag  = !isr_flag;
	//if(SPIC.STATUS & 0x40)
	//	ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
	rx_byte = SPIC.DATA;
	spic_flag = 1;
	
	/*isr_flag = 1;
	if(rx_byte == 0xAA){
		start_tx = 1;
	}
	
	if(start_tx){
		//Send normal data
		if(data_idx<data_len){
			sent_buff[data_idx] = rx_buff[data_idx];
			SPIC.DATA = rx_buff[data_idx];
		}
		else if(data_idx<data_len+2){//Send checksum
			//ioport_set_pin_level(LED_0_PIN,LED_0_ACTIVE );
			if(data_idx == data_len){//Compute checksum
				//checksum = crc_io_checksum(sent_buff, data_len, CRC_16BIT);
			}
			//Send checksum byte by byte
			SPIC.DATA = (uint8_t)(checksum>> (8 * (data_idx-data_len)));
			
		}
		else{//Send data and checksum, done with transfer
			start_tx = 0; //reset
			data_idx = 0; //reset
			SPIC.DATA = 0x00;//Send 0
			return;
		}
		
		
		data_idx++;
	}
	//if(rx_byte == 0xde){
	
	
	/*if(isr_flag){
		SPIC.DATA = 0xAA;//(uint8_t)(checksum >> (8*i));
		//i++;
		//i &=1;
	}else{
		SPIC.DATA = 0xBB;//(uint8_t)(checksum >> (8*i));
		while(!(SPIC.STATUS & 0x80));
		SPIC.DATA = 0xCC;//(uint8_t)(checksum >> (8*i));
		while(!(SPIC.STATUS & 0x80));
		SPIC.DATA = 0xDD;//(uint8_t)(checksum >> (8*i));
		//i++;
		//i &=1;
		//i = 0;
	}
	*/
	
	
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
			}
			
			//If we are receiving command, store it appropriately
			if(cmd_idx > 0){
				cmd_data[CMD_DATA_SIZE-cmd_idx] = rx_byte;
				cmd_idx--;
				
				//Finished last storage of incoming data
				//Now check CRC
				if(cmd_idx == 0){
					received_crc =	(cmd_data[CMD_DATA_SIZE-1]<<8) | cmd_data[CMD_DATA_SIZE-2];
					calculated_crc = crc_io_checksum(cmd_data, CMD_DATA_SIZE-2, CRC_16BIT);
					if(calculated_crc == received_crc){
						ioport_set_pin_level(LED_0_PIN,LED_0_ACTIVE);
						SPIC.DATA = 0xAA;
					}
					else{
						SPIC.DATA = 0xFF;
					}
				}				
			}
			
			
			
			spic_flag = 0;
		}
		
			
	}
}