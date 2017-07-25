/*
 * i2c_sensors.h
 *
 * Created: 7/22/2017 4:05:19 PM
 *  Author: rgw3d
 */ 


#ifndef I2C_SENSORS_H_
#define I2C_SENSORS_H_

#include <asf.h>
#include <string.h>

#define ADC_STREAMING 0x42
#define ADC_SINGLE    0x43
#define AIN0  0xc3
#define AIN1  0xd3
#define AIN2  0xe3
#define AIN3  0xf3

void init_chip(TWI_t *twi, uint8_t chip);

//
//
//ADC 
//
//


twi_package_t adc_write; 
twi_package_t adc_read; 

/* 0b01000010 == 0x42
 * 15:Operating status. Reading this has value. writing 0 does nothing
 * 14:12 Input mux. 000=AIN0
 * 11:9  PGA config. 001=+4.096V
 * 8:Mode 0=Continuous-conversion
 * 0b11100011 == 0x83
 * 7:5 Data Rate: 111=860 SPS
 * 4: Comparator mode. 0=default
 * 3: Comparator polarity. 0=default
 * 2: Comparator Latch. 0=Non latching comparator
 * 1: Comparator queue and disable. 00=Assert after one conversion
 **/
int8_t init_adc(TWI_t *twi, uint8_t chip, uint8_t type_of_adc);
int8_t set_adc_mux(TWI_t * twi, uint8_t chip, uint8_t mux);
extern int8_t read_adc(TWI_t *twi, uint8_t chip, void * buff );






#endif /* I2C_SENSORS_H_ */
