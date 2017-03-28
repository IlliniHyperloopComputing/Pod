#ifndef SPI_H 
#define SPI_H 

#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <string>

using namespace std;


enum Xmega_Request_t {
  //Read num bytes as described in Xmega_Setup
  X_SENSOR = 0,
  //Read byte to determine if any Xmega sensor error
  X_SENSOR_STATUS = 1,
  //What state is the Xmega in. Reads a byte
  X_STATE = 2,
  //Read All of the above at once
  X_ALL = 3,
};

typedef struct Xmega_Request_{

} Xmega_Request;

typedef struct Xmega_Setup_{
  /**
  * Spi file path
  * "/dev/spidev1.0"
  * "/dev/spidev1.1"
  * This will likely be static memory
  **/
  string file_path;
  
  //Number of bytes to read when making sensor request 
  uint8_t sensor_request_num_bytes;

  /**
  * Number of data items. 
  * Say the data to read from the Xmega contains
  * 3 thermocouples, a distance value, and an ADC value, 
  * then num_items == 5
  **/
  uint8_t num_items;

  /**
  * An array of length num_items
  * each index holds the number of bytes that the corresponding
  * takes up in memory, even if the data doesn't use all bits. 
  * For example, if the thermocouple data is 16 bits, then the 
  * corresponding value is 2. 
  * For example, if some ADC value is known to only be 10-bits,
  * 2 bytes will still be used to represent this value. So the 
  * corresponding value will be 2.
  * 
  * Example: 
  * num_items = 4
  * index         -> |       0       |         1        |     2      |     3      |
  * xmega data    -> | 16-bit thermo | 8-bit rideheight | 10-bit ADC | 12-bit ADC |
  * bytes_per_item-> |       2       |         1        |     2      |     2      |
  *
  * Question: Why pad the 10-bit values with "useless" bits to make up a byte? 
  *           Why not just 'stack' them, crossing byte boundaries? 
  *           Say we had three 10-bit values to send. 'stack'ing 
  *           them requires a total of 30 bits, or 4 bytes. 
  *           Not 'stack'ing them requires 6 bytes.
  *           Wouldn't this save on transmission time? 
  *
  * Answer:   Yes. But its a pain to pull out each data item and is easily 
  *           prone to error. Also, the savings on transmission for 20 bytes
  *           is approximatly .5 milliseconds. So it only makes sense to
  *           implement when there is ALOT of data that can be 'stack'ed
  *           But it's probably just better to keep the code readable,
  *           not to mention it's much easier to do on the Xmega without
  *           'stack'ing.
  * 
  **/
  uint8_t * bytes_per_item;

  //Speed in Hz. Should be 500000;
  uint32_t speed;

  //Bits per word. 8
  uint32_t bits_per_word;

} Xmega_Setup;

class Spi {
  
  public: 

    /**
    * Construct an Spi object
    * @param x1  Xmega #1 setup
    * @param x2  Xmega #2 setup
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
    * @param request_type   Xmega_Request describing request type from 
    *                       each Xmega.
    **/
    void request(Xmega_Request request_type);

    /**
    * Of the most recently recieved data, get the appropriate index
    * Refer to the bytes_per_item member of the Xmega_Setup struct
    * @param device  which Xmega's data? 
    *                Value of 0 or 1
    *
    * @param idx     which index to pull? 
    *                Max value of Xmega_Setup(device).num_items-1
    *                
    **/
    uint32_t get_data(uint8_t device, int idx);

  private:

    /**
    * Setup SPI for each Xmega. Should only be called after 
    * Xmega_Setup is stored
    **/
    void setup_spi();


    //Storage of setup details 
    Xmega_Setup * x1;
    Xmega_Setup * x2;

    //storage of most recently read in data
    //dynamically allocated, acording to maximum message size + 2
    uint8_t * x1_buff;
    uint8_t * x2_buff;

    /**
    * To quickly find each data item, and to make the code simpler,
    * this will hold forward calculations of all the offsets. 
    *
    * Example: 
    * num_items = 4
    * index         -> |       0       |         1        |     2      |     3      |
    * xmega data    -> | 16-bit thermo | 8-bit rideheight | 10-bit ADC | 12-bit ADC |
    * bytes_per_item-> |       2       |         1        |     2      |     2      |
    * offset_lookup -> |       0       |         2        |     3      |     5      |
    *
    **/
    uint8_t * x1_offset_lookup;
    uint8_t * x2_offset_lookup;

};

#endif 
