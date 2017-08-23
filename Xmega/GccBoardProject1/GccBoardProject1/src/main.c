
#include <asf.h>

struct spi_device spi_pf0 = {
	.id = IOPORT_CREATE_PIN(PORTF, 0)
};
struct spi_device spi_pf1 = {
	.id = IOPORT_CREATE_PIN(PORTF, 1)
};
struct spi_device spi_pf2 = {
	.id = IOPORT_CREATE_PIN(PORTF, 2)
};
struct spi_device spi_pf3 = {
	.id = IOPORT_CREATE_PIN(PORTF, 3)
};
struct spi_device spi_pr0 = {
	.id = IOPORT_CREATE_PIN(PORTR, 0)
};

void spi_init_pins(void)
{
	//Chip select pin PF0
	ioport_configure_port_pin(&PORTF, PIN0_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT); 
	//Chip select pin PF1
	ioport_configure_port_pin(&PORTF, PIN1_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PF2
	ioport_configure_port_pin(&PORTF, PIN2_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PF3
	ioport_configure_port_pin(&PORTF, PIN3_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	//Chip select pin PR0
	ioport_configure_port_pin(&PORTR, PIN0_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
	
	//The SS pin MUST be high. otherwise will switch into slave mode
	//PF4
	ioport_configure_port_pin(&PORTF, PIN4_bm, IOPORT_PULL_UP | IOPORT_DIR_INPUT);
	
	//MISO PF6
	ioport_configure_port_pin(&PORTF, PIN6_bm, IOPORT_DIR_INPUT); 
	//SCK PF7
	ioport_configure_port_pin(&PORTF, PIN7_bm, IOPORT_INIT_HIGH | IOPORT_DIR_OUTPUT);
}
void spi_init_module(void)
{
	
	spi_master_init(&SPIF);
	
	spi_master_setup_device(&SPIF, &spi_pf0, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pf1, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pf2, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pf3, SPI_MODE_0, 1000000, 0);
	spi_master_setup_device(&SPIF, &spi_pr0, SPI_MODE_0, 1000000, 0);
	
	
	spi_enable(&SPIF);
}

int main (void)
{
	board_init();
	spi_init_pins();
	spi_init_module();

	uint8_t data_buffer[2] = {0xAB,0xAB};

	while (1) {
		spi_select_device(&SPIF, &spi_pf0);
		spi_read_packet(&SPIF, data_buffer, 2);
		spi_deselect_device(&SPIF, &spi_pf0);
		
		uint16_t out = (data_buffer[0] | (data_buffer[1]<<8)>>3);
		out /=4;
	//	spi_select_device(&SPIF, &spi_pf1);
	//	spi_read_packet(&SPIF, data_buffer, 2);
	//	spi_deselect_device(&SPIF, &spi_pf1);
	//	
	//	spi_select_device(&SPIF, &spi_pf2);
	//	spi_read_packet(&SPIF, data_buffer, 2);
	//	spi_deselect_device(&SPIF, &spi_pf2);
	//	
	//	spi_select_device(&SPIF, &spi_pf3);
	//	spi_read_packet(&SPIF, data_buffer, 2);
	//	spi_deselect_device(&SPIF, &spi_pf3);
	//	
	//	spi_select_device(&SPIF, &spi_pr0);
	//	spi_read_packet(&SPIF, data_buffer, 2);
	//	spi_deselect_device(&SPIF, &spi_pr0);
		
	}
}
