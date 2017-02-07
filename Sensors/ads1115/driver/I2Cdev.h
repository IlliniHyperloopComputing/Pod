#ifndef _I2CDEV_H_
#define _I2CDEV_H_

#include <string>
#include <iostream>
#include <string>
#include <atomic>
#include <linux/i2c-dev.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/time.h>
/*
    readWord
    readBitW
    readBitsW
    writeBitW
    writeBitsW
    writeWord

*/
class I2Cdev{
    public:
        static int8_t readBitW(int devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data);
        static int8_t readBitsW(int devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data);
        static int8_t readWord(int devAddr, uint8_t regAddr, uint16_t *data);

        static uint8_t writeBitW(int devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
        static uint8_t writeBitsW(int devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
        static uint8_t writeWord(int devAddr, uint8_t regAddr, uint16_t data);
};

#endif /* _I2CDEV_H_ */
