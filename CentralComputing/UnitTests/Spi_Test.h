//Create header guards
#ifndef _SPI_TEST_H
#define _SPI_TEST_H

//Need to include Unit_Test in order to extend it
#include "Unit_Test.h"

//Publicly extend Unit_Test
class Spi_Test : public Unit_Test {
  public: 
    /**
    * Implementing test() of virtual function
    **/
    int test(int argc, char** argv);

    /**
    * Use the constructor to define name and cmd fields
    **/
    Spi_Test();

  //Any additional functions should be defined in the private section
  //private: 
};

#endif
