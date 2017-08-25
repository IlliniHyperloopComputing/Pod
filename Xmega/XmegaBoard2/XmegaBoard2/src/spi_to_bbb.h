/*
 * spi_to_bbb.h
 *
 * Created: 4/20/2017 11:50:02 PM
 *  Author: rgw3d
 */ 

#include <asf.h>
#include <avr/interrupt.h>
#include <string.h>//memcpy
#include "circular_buffer.h"
#define SENSOR_DATA_SIZE 29

#ifndef SPI_TO_BBB_H_
#define SPI_TO_BBB_H_

void init_spi_to_bbb(void);
void handle_spi_to_bbb(void);

#endif /* SPI_TO_BBB_H_ */
