#ifndef CRC_H
#define CRC_H

#include <unistd.h>
#include <stdint.h>

static uint16_t CRCCCITT(uint8_t *data, size_t length, uint16_t seed);

static uint16_t CRCCCITT_byte(uint16_t crc, uint16_t data);


#endif
