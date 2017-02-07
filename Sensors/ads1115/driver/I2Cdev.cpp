#include "I2Cdev.h"

/** Read single word from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param data Container for word value read from device
 * @return Status of read operation (TRUE = success)
 */
int8_t I2Cdev::readWord(int devAddr, uint8_t regAddr, uint16_t *data) {
    //uint8_t buf = I2Cdev_readWords(devAddr, regAddr, 1, data);
    int16_t tmp = i2c_smbus_read_word_data(devAddr, regAddr);
    data[0] = (uint16_t)tmp;
    return tmp>0;
}


/** Read a single bit from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-15)
 * @param data Container for single bit value
 * @return Status of read operation (TRUE = success)
 */
int8_t I2Cdev::readBitW(int devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data) {
    uint16_t b;
    uint8_t count = readWord(devAddr, regAddr, &b);
    *data = b & (1 << bitNum);
    return count;
}

/** Read multiple bits from a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-15)
 * @param length Number of bits to read (not more than 16)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @return Status of read operation (1 = success, 0 = failure, -1 = timeout)
 */
int8_t I2Cdev::readBitsW(int devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data) {

    // 1101011001101001 read byte
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    //    010           masked
    //           -> 010 shifted
    uint8_t count;
    uint16_t w;
    if ((count = readWord(devAddr, regAddr, &w)) != 0) {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        w &= mask;
        w >>= (bitStart - length + 1);
        *data = w;
    }

    return count;
}

/** Write single word to a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register address to write to
 * @param data New word value to write
 * @return Status of operation (TRUE = success)
 */
uint8_t I2Cdev::writeWord(int devAddr, uint8_t regAddr, uint16_t data) {
    //uint8_t buf = I2Cdev_writeWords(devAddr, regAddr, 1, &data);
    int16_t tmp = i2c_smbus_write_word_data(devAddr, regAddr, data);
    return tmp>0;
}

/** write a single bit in a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-15)
 * @param value New bit value to write
 * @return Status of operation (TRUE = success)
 */
uint8_t I2Cdev::writeBitW(int devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data) {

    uint16_t w;
    readWord(devAddr, regAddr, &w);
    w = (data != 0) ? (w | (1 << bitNum)) : (w & ~(1 << bitNum));
    uint8_t buf = writeWord(devAddr, regAddr, w);

    return buf;
}


/** Write multiple bits in a 16-bit device register.
 * @param devAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitStart First bit position to write (0-15)
 * @param length Number of bits to write (not more than 16)
 * @param data Right-aligned value to write
 * @return Status of operation (TRUE = success)
 */
uint8_t I2Cdev::writeBitsW(int devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data) {


    //              010 value to write
    // fedcba9876543210 bit numbers
    //    xxx           args: bitStart=12, length=3
    // 0001110000000000 mask word
    // 1010111110010110 original value (sample)
    // 1010001110010110 original & ~mask
    // 1010101110010110 masked | value
    uint16_t w;
    if (readWord(devAddr, regAddr, &w) != 0) {
        uint16_t mask = ((1 << length) - 1) << (bitStart - length + 1);
        data <<= (bitStart - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        w &= ~(mask); // zero all important bits in existing word
        w |= data; // combine data with existing word
        uint8_t buf = writeWord(devAddr, regAddr, w);

        return buf;
    } else {

        return 0;
    }
}
