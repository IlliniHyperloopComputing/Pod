/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <avr/interrupt.h>

//Using crc-ccitt
#include <stdbool.h>
#include <assert.h>

volatile uint8_t rx_byte = 0x00;
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
	
	uint8_t to_crc = 0xde;
	
	checksum = crc_io_checksum(rx_buff, 12, CRC_16BIT);
	
	
	SPIC.DATA = 0xff;
	
	while (1) {
			
	}
}
