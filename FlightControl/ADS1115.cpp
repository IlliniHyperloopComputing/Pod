#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "ADS1115.h"

/**
 * @brief ADS1115 constructor
 * Default gain is 4096
 * Default multiplexer is between GND and P0
 * Default comparator mode is disabled
 * Default mode is singleshot
 * @param address
 */
ADS1115::ADS1115(uint8_t fileDescriptor,int address) {
    this->address = address;
    this->fileDescriptor = fileDescriptor;
    memset(&config, 0, sizeof(config));
    setGain(ADS1115_PGA_4P096);
    setMultiplexer(ADS1115_MUX_P0_NG);
    setMode(ADS1115_MODE_SINGLESHOT);
    setComparatorQueueMode(ADS1115_COMP_QUE_DISABLE);
}

ADS1115::~ADS1115() {

}

/** Verify the I2C connection.
* @return True if connection is valid, false otherwise
*/
bool ADS1115::testConnection() {
    uint8_t data;
	int8_t status = i2c_smbus_read_byte_data(fileDescriptor, ADS1115_RA_CONFIG);
    //int8_t status = I2Cdev::readByte(address, ADS1115_RA_CONFIG, &data);
    if (status > 0)
        return true;
    else
        return false;
}

/**
 * @brief Call it if you updated ConfigRegister
 */
void ADS1115::updateConfigRegister() {
    uint16_t c;

    /* New config */
    c = config.status | config.mux | config.gain |
        config.mode | config.rate | config.comparator |
        config.polarity | config.latch | config.queue;
	if (i2c_smbus_write_word_data(fileDescriptor, ADS1115_RA_CONFIG, c) < 0){
    //if ( I2Cdev::writeWord(address, ADS1115_RA_CONFIG, c) < 0) {
        fprintf(stderr, "Error while writing config\n");
    }
}

/**
 * @brief Get data from Conversion Register
 *
 * @return Little-Endian result
 */
int16_t ADS1115::getConversion() {
    union {
        uint16_t w;
        uint8_t b[2];
    } word;
    word.w = 0;

    if (config.mode == ADS1115_MODE_SINGLESHOT ) {
        /* Check for Operation Status. If it is 0 then we are ready to get data. Otherwise wait. */
        setOpStatus(ADS1115_OS_ACTIVE);
        while ((word.w & 0x80) == 0) {
            int16_t config_data = i2c_smbus_read_word_data(fileDescriptor,ADS1115_RA_CONFIG);
            word.w = static_cast<uint16_t>(config_data);
			if (config_data < 0)
                fprintf(stderr, "Error while reading config, address %x\n",address); 
        } 
    } 
    int16_t config_data = i2c_smbus_read_word_data(fileDescriptor, ADS1115_RA_CONVERSION);
    word.w = static_cast<uint16_t>(config_data);
            
	if (config_data<0){
        fprintf(stderr, "Error while reading\n");
    }
    /* Exchange MSB and LSB */
    word.w = word.b[0] << 8 | word.b[1];
    return static_cast<int16_t> (word.w);
}

/**
 * @brief Update Operational Status
 *
 * @param Desired Status
 */
void ADS1115::setOpStatus(uint16_t status) {
        config.status = status;
        updateConfigRegister();
}

/**
 * @brief Check which Multiplexer is selected
 *
 * @return Multiplexer status
 */
uint16_t ADS1115::getMultiplexer() {
    return config.mux;
}

/**
 * @brief Choose the multiplexer
 *
 * @param Desired multiplexer
 */
void ADS1115::setMultiplexer(uint16_t mux) {
    if (config.mux != mux) {
        config.mux = mux;
        updateConfigRegister();
    }
}

/**
 * @brief Get current gain
 *
 * @return Current Gain
 */
uint16_t ADS1115::getGain() {
    return config.gain;
}

/**
 * @brief Set gain
 *
 * @param gain
 */
void ADS1115::setGain(uint16_t gain) {
    if (config.gain != gain) {
        config.gain = gain;
        updateConfigRegister();
    }
}

/**
 * @brief Get mode
 *
 * @return mode
 */
uint16_t ADS1115::getMode() {
    return config.mode;
}

/**
 * @brief Set mode
 *
 * @param mode
 */
void ADS1115::setMode(uint16_t mode) {
    if (config.mode != mode) {
        config.mode = mode;
        updateConfigRegister();
    }
}

/**
 * @brief Get rate
 *
 * @return rate
 */
uint16_t ADS1115::getRate() {
    return config.rate;
}

/**
 * @brief Set rate
 *
 * @param rate
 */
void ADS1115::setRate(uint16_t rate) {
    if (config.rate != rate) {
        config.rate = rate;
        updateConfigRegister();
    }
}

/**
 * @brief Show content of config register
 */
void ADS1115::showConfigRegister() {
    union {
        uint16_t w;
        uint8_t b[2];
    } buf;
	buf.w = i2c_smbus_read_word_data(fileDescriptor,ADS1115_RA_CONFIG);
    //I2Cdev::readWord(address, ADS1115_RA_CONFIG, &buf.w);
    printf("Config Register: 0x%04x | 0x%02x 0x%02x\n", buf.w, buf.b[0], buf.b[1]);
}

/**
 * @brief Get content of conversion register in mV. It gets converted using current gain
 * @see setGain
 *
 * @return Last conversion in mV
 */
float ADS1115::getMilliVolts() {
  switch (config.gain) {
    case ADS1115_PGA_6P144:
      return (getConversion() * ADS1115_MV_6P144);
      break;
    case ADS1115_PGA_4P096:
      return (getConversion() * ADS1115_MV_4P096);
      break;
    case ADS1115_PGA_2P048:
      return (getConversion() * ADS1115_MV_2P048);
      break;
    case ADS1115_PGA_1P024:
      return (getConversion() * ADS1115_MV_1P024);
      break;
    case ADS1115_PGA_0P512:
      return (getConversion() * ADS1115_MV_0P512);
      break;
    case ADS1115_PGA_0P256:
    case ADS1115_PGA_0P256B:
    case ADS1115_PGA_0P256C:
      return (getConversion() * ADS1115_MV_0P256);
      break;
    default:
      fprintf(stderr, "Wrong gain\n");
      return -1;
      break;
  }
}

/**
 * @brief set comparator mode
 *
 * @param comparator
 */
void ADS1115::setComparatorMode(uint16_t comparator) {
    if (config.comparator != comparator) {
        config.comparator = comparator;
        updateConfigRegister();
    }
}

/**
 * @brief Set Comparator polarity
 *
 * @param polarity
 */
void ADS1115::setComparatorPolarity(uint16_t polarity) {
    if (config.polarity != polarity) {
        config.polarity = polarity;
        updateConfigRegister();
    }
}

/**
 * @brief Set comparator latch status
 *
 * @param latch
 */
void ADS1115::setComparatorLatchEnabled(uint16_t latch) {
    if (config.latch != latch) {
        config.latch = latch;
        updateConfigRegister();
    }
}

/**
 * @brief Set Comparator Queue Mode
 *
 * @param queue
 */
void ADS1115::setComparatorQueueMode(uint16_t queue) {
    if (config.queue != queue) {
        config.queue = queue;
        updateConfigRegister();
    }

}

void ADS1115::updateHiThreshRegister(uint16_t value){
	if (i2c_smbus_write_word_data(fileDescriptor, ADS1115_RA_HI_THRESH, value) < 0){
        fprintf(stderr, "Error while writing HI THRESH\n");
    }
	updateConfigRegister();
}

void ADS1115::updateLoThreshRegister(uint16_t value){
	if (i2c_smbus_write_word_data(fileDescriptor, ADS1115_RA_LO_THRESH, value) < 0){
        fprintf(stderr, "Error while writing LO THRESH\n");
    }
	updateConfigRegister();
}

void ADS1115::setComparatorHiThresh(uint16_t thresh, uint16_t gainInVolts) {
	//uint16_t value = gainInVolts/(hiThreshInMilliVolts/1000.0/32768.0);
	//updateHiThreshRegister(value);
	updateHiThreshRegister(thresh);//*gainInVolts/32768.0);
}

void ADS1115::setComparatorLoThresh(uint16_t loThreshInMilliVolts, uint16_t gainInVolts) {
	uint16_t value = gainInVolts/(loThreshInMilliVolts/1000.0/32768.0);
	updateLoThreshRegister(value);
}
