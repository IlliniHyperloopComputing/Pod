#ifndef SPI_H 
#define SPI_H 

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

//
enum Xmega_Request_t {
  //Read num bytes as described in Xmega_Setup
  X_SENSOR = 0,
  //Read byte to determine if any Xmega sensor error
  X_SENSOR_STATUS = 1,
  //What state is the Xmega in. Reads a byte
  X_STATE = 2,
  //Read All of the above at once
  X_ALL = 3,
}

typedef struct Xmega_Setup_{
  //Spi file path
  // "/dev/spidev1.0"
  char * file_path;
  
  //Number of bytes to read when making sensor request 
  uint8_t sensor_request_num_bytes;

  //number of data items. 
  //if we were getting the values of 3 thermocouples,
  //a distance value, and an ADC value, then
  //num_items == 5
  uint8_t num_items;


  /**
  * An array of length num_items
  * each index holds the number of bits that the corresponding
  * data is. So, if the thermocouple data is 16 bits, then the
  * correpsonding index is 16. 
  * 
  * num_items = 4
  * index        -> |   0    |      1      |     2      |     3      |
  * xmega data   -> | thermo | ride height | 10-bit ADC | 12-bit ADC |
  * bits_per_item-> |   16   |      16     |     10     |     16     |
  * 
  **/
  uint8_t * bits_per_item;

  //Speed in Hz. Should be 500000;
  uint32_t speed;

  //Bits per word. 8
  uint32_t bits_per_word;
} Xmega_Setup;

class Spi {
  
  public: 

    /**
    * Construct an Spi object
    * @param  Xmega #1 setup
    * @param  Xmega #2 setup
    **/ 
    Spi(Xmega_Setup * x1, Xmega_Setup * x2);

    /**
    * Destructor
    **/
    ~Spi();

    /**
    * Send/Recieve data from the Xmegas
    * Communications between each Xmega is interlaced
    * because of corruption issues experienced before
    * @param Enum describing request type. 
    **/
    request(Xmega_Request request_type);

    get_data(uint8_t device, int idx);

  private:

    //Storage of setup details
    Xmega_Setup x1;
    Xmega_Setup x2;

    //storage of last read in stuff
    //dynamically allocated, acording to maximum message size + 2
    char * x1_buff;
    char * x2_buff;

}



#endif 
