#ifndef SPI_H 
#define SPI_H 

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <string>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <unistd.h>

#include "Crc.h"

#define CRC_PASS 0xAA
#define SLEEP_TIME 10

using namespace std;

enum Xmega_Transmission_Failure_t: uint8_t {
  //No error
  X_TF_NONE = 0,
  //Error sending data
  X_TF_SEND = 1,
  //Error recieving data
  X_TF_RECIEVE =2,
};

enum Xmega_Command_t: uint8_t {
  //No Command
  X_C_NONE = 0,
  //TODO: Add others as needed
};

enum Xmega_Request_t: uint8_t {
  //Read num bytes as described in Xmega_Setup
  X_R_SENSOR = 0,
  //Read byte to determine if any Xmega sensor error
  X_R_SENSOR_STATUS = 1,
  //What state is the Xmega in. Reads a byte
  X_R_STATE = 2,
  //Read All of the above at once
  X_R_ALL = 3,
  //None
  X_R_NONE = 4,
};

typedef struct Xmega_Transfer_{
  //Which device? 0 or 1
  int device;
  //What data are we sending/commanding
  enum Xmega_Command_t cmd;
  //What data are we requesting
  enum Xmega_Request_t req;
  
} Xmega_Transfer;

typedef struct Xmega_Setup_{
  /**
  * Spi file path
  * "/dev/spidev1.0"
  * "/dev/spidev1.1"
  **/
  const char * file_path;

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

typedef struct Xmega_Data_ {

  //SPI file descriptors for each Xmega
  int fd;

  //storage of most recently read in data
  //dynamically allocated, according to maximum message size + 2
  uint8_t * buff;

  //Number of total bytes used by data (not CRC, status, or state)
  //in buff
  uint8_t num_bytes;

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
  uint8_t * offset_lookup;

  /**
  * Storage for most recent byte describing Xmega sensor_status / state
  * This information will be initially transfered into the buffer, 
  * but moved here because sensor_status and state might not be 
  * requested every time
  **/
  uint8_t sensor_status;
  uint8_t state;

} Xmega_Data;

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
    * Interesting notes about reading in data (and transfers in general)
    * (Note: when I say Xmega, I mean the Xmega A1U Xplained Pro board)
    *
    * Speed: The BBB is capable of 24000000 Hz clock for SPI. That is 
    * silly fast. That is at most 3 Mb/s, Realistically around 1 Mb/s.
    * Damn. 
    * Much time was spent banging my head against the Xmega until I 
    * realized that the Maximum possible speed was 1000000 Hz on it. 
    * But 1000000 is still a little too fast for some reason. In theory
    * It should work on the Xmega, but emperical evidence shows otherwise.
    * There is frequent corruption of the bytes recieved, with no identifiable 
    * pattern. So I lowered the clock to 500000 Hz, and that is the sweet spot. 
    *
    * Read size: Currently the read()s are being made byte-by-byte using
    * read(,,1). Why not use read(,,total_bytes_to_read)? Because after 
    * the fourth hour of debugging _why the hell_ it wasn't working, using
    * read(,,1) isn't so bad. And read(,,1) has not failed me yet. I did not
    * have an oscilliscope on hand, and could not debug why exactly it was
    * not working. I might try later, but the current strategy works. Also,
    * the time savings are not that huge. Reading a total of 32 bytes, 
    * 16 bytes from each Xmega, takes ~1.2 milliseconds. That is acceptable
    * for me. 
    * Might as well include this test I ran a while ago. These did not 
    * account for any data corruption. 
    * 500 kHZ clock
    * ~ .000059 seconds for repeat write(), 1 byte
    * ~ .000061 seconds for repeat read(), 1 byte
    * ~ .000155 seconds for single write(), 1 byte
    * ~ .000156 seconds for single read(), 1 byte
    *     6410 bytes/second
    *
    * ~ .000106 seconds for repeat write(), 4 bytes
    * ~ .000108 seconds for repeat read(), 4 bytes
    * ~ .000207 seconds for single write(), 4 bytes
    * ~ .000210 seconds for single read(), 4 bytes
    *     19047 bytes/second
    *
    * ~ .000800 seconds for single read(), 32 bytes
    * ~ .002000 seconds for single read(), 64 bytes
    *     32000 bytes/second
    *
    * Sequential Read()s: I have not experienced this problem with sequential
    * writes, but with sequential reads without enough time inbetween, 
    * there tends to be corruption. This might just be a problem with the 
    * code on the Xmega, since it is in an early stage. Here is how I 
    * figured this one out: just a for{} loop only containing read()s and
    * a fprintf() for debugging. This worked perfectly. As I remove the 
    * print, it breaks. The version with fprintf() worked 99/100 times.
    * The version without works 10/100 times. (That is, without corruption)
    * Technically this works, as long as there is a CRC checksum, but it's
    * pretty horrible. The average read (with printf) of 16 bytes took
    * about .5 millisec. The average read (without printf) of 16 bytes took
    * about 5 or 6 millisec. Horrible! (Sad!) 
    * To solve this problem the reads for each Xmega are interlaced. That
    * seems to do the trick. Using usleep() seems like it would also work.
    *
    *
    *
    * First, write the appropriate data.
    * 1 start byte, 1 command byte, 1 request byte, and 2 bytes for CRC
    * Using 0xAA as start byte. Why? Because command or request will never
    * equal 0xAA. Also, 0xAA is easy to see on an Oscilliscope, and 
    * less likely to appear by mistake than 0xFF or 0x00 ... at least
    * I think so. Whatever. 
    *
    * A start byte is needed so the Xmega knows a transfer is about
    * to happen, and can start pipelining the correct data. 
    * 
    * Need to send 5 bytes before reading
    * Write -> | 0xAA | command | request | CRC | CRC |
    * 
    * Now onto reading. No writing will take place during this time, just
    * need to get all of the data in. 
    * Need to read amount depending on request type
    * Read  -> | data | data | data ..... | CRC | CRC | 
    *
    * Note: None of the transfers are full duplex. They are all half duplex.
    *       It is simpler this way. Full duplex is great when we have alot
    *       to transfer from each side. In our situation we spend a majority
    *       of the time reading, and thus half duplex is simpler.
    *
    * @param request_type   Xmega_Transfer describing transfer
    * @return failure mode   0b00000000 == no failure
    *                        0b00000001 == x1 send failure
    *                        0b00000010 == x2 send failure
    *                        0b00000100 == x1 recieve failure
    *                        0b00001000 == x2 recieve failure
    *                        These codes are bitwise ORed together
    *                        Send/Recieve errors from the same board are exclusive
    * 
    **/
    int transfer(Xmega_Transfer &transfer);

    /**
    * Of the most recently recieved data, get the appropriate index
    * Refer to the bytes_per_item member of the Xmega_Setup struct
    * @param device  which Xmega's data? 
    *                Value of 0 or 1
    *
    * @param idx     which index to pull? 
    *                Max value of Xmega_Setup(device).num_items-1
    *                
    * @return uint32 containing requested data. Largest data is 32 bits
    **/
    uint32_t get_data(uint8_t device, int idx);

    /**
    * Getter function returning the most recent sensor status
    * of the specified device.
    * No Spi transfer happens here.
    * @param device   which Xmega's data?
    *                 Value of 0 or 1 
    **/
    uint8_t get_sensor_status(uint8_t device);

    /**
    * Getter function returning the most recent state of the 
    * specified device.
    * No Spi transfer happens here.
    * @param device   which Xmega's data?
    *                 Value of 0 or 1 
    **/
    uint8_t get_state(uint8_t device);

  private:

    /**
    * Setup SPI for each Xmega. Should only be called after 
    * Xmega_Setup is stored.
    * @return integer describing sucess. 0==success, 1==failure
    **/
    int setup_spi();

    /**
    * Used to setup the offset field that is part of each xmega data
    * struct. Really it just makes the constructor cleaner
    *
    **/
    void setup_offset(Xmega_Data * xd, Xmega_Setup * xs);

    //Storage of setup details 
    Xmega_Setup * x1_setup;
    Xmega_Setup * x2_setup;

    //Storage of all data that isn't data
    Xmega_Data * x1;
    Xmega_Data * x2;

};

#endif 
