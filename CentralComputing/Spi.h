#ifndef SPI_H 
#define SPI_H 

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

enum Xmega_Request {
  //Read num bytes as described in Xmega_Setup
  SENSOR = 0,
  //Read byte to determine if any Xmega sensor error
  SENSOR_STATUS = 1,
  //What state is the Xmega in
  STATE = 2,
  
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
    **/ 
    Spi(Xmega_Setup * x1, Xmega_Setup * x2);

    /**
    * Destructor
    **/
    ~Spi();



  private:

    //Storage of setup details
    Xmega_Setup x1;
    Xmega_Setup x2;

}



#endif 
