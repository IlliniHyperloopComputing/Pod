/*
 * spi_sensors.h
 *
 * Created: 7/23/2017 4:02:06 PM
 *  Author: rgw3d
 */ 


#ifndef SPI_SENSORS_H_
#define SPI_SENSORS_H_

#include <asf.h>


struct spi_device spi_pf0;
struct spi_device spi_pf1;
struct spi_device spi_pf2;
struct spi_device spi_pf3;

void init_thermo_sensors(void);


//Pass in 32 bit value from temperature sensor to decode. Get 16 bit out
int16_t thermo_external_temp(uint32_t v);

//Pass in 32 bit value from temperature sensor to decode. Get 16 bit out
int16_t thermo_internal_temp(uint32_t v);

//Get the raw value from the thermocouple
uint32_t read_thermo(uint8_t device);

#endif /* SPI_SENSORS_H_ */
