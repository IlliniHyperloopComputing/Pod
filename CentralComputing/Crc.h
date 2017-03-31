#ifndef CRC_H
#define CRC_H

#include <unistd.h>
#include <stdint.h>

class Crc{
  public:

    /**
    * The CRC algroithm used is the CRC-CCITT XModem
    * Why this one? Because the Xmega has hardware 
    * acceleration for this calculation 
    * Also, this version is only 16 bits, not 32.
    * This impacts transmission time, since a CRC
    * checksum will be sent following every message
    * is only 16 bits, as opposed to 32 bits. 
    *
    * @param data     array of bytes to be processed
    * @param length   length of data array
    * @param seed     initial CRC value. Should be 0 
    *                   to comply with XModem
    *
    **/
    static uint16_t CRCCCITT(uint8_t *data, size_t length, uint16_t seed);

    static uint16_t CRCCCITT_byte(uint16_t crc, uint16_t data);

    //This mysterious table holds all of CRC calculations
    //So they do not need to be recomputed
    const static uint16_t crc_table[];
};

#endif
