#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <linux/types.h>
#include <linux/spi/spidev.h>

uint16_t CRCCCITT(uint8_t *data, size_t length, uint16_t seed);

uint16_t CRCCCITT_byte(uint16_t crc, uint16_t data);
