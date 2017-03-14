#ifndef SPI_H 
#define SPI_H 

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

//
enum Xmega_Request {
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
    * Send a request to the Xmega for data. Also read it back
    * @param Enum describing request type. Doubles as the request code
    * @param Device number 0 or 1. Depends on constructor initalization order
    **/
    request(Xmega_Request request_type, uint8_t device);


    



  private:

    //Storage of setup details
    Xmega_Setup x1;
    Xmega_Setup x2;

}



#endif 
