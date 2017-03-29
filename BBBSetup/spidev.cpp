#include "BlackLib/v3_0/BlackCore.h"
#include "BlackLib/v3_0/BlackSPI/BlackSPI.h"
#include <iostream>


int main(){
	BlackLib::BlackSPI  mySpi(BlackLib::SPI0_0, 8, BlackLib::SpiDefault, 2400000);
	if( mySpi.fail(BlackLib::BlackSPI::dtErr)  )
	{
	    std::cout << "BlackSPI INITIALIZATION FAILED dtErr" << std::endl;
	}
	else if(mySpi.fail(BlackLib::BlackSPI::portPathErr))
	{
	    std::cout << "BlackSPI INITIALIZATION FAILED portPathErr" << std::endl;
	    std::cout << "BlackSPI INITIALIZATION IS OK" << std::endl;
	}
	else{
	    std::cout << "BlackSPI INITIALIZATION IS OK" << std::endl;
	}
	mySpi.open( BlackLib::ReadWrite | BlackLib::NonBlock );
	if( mySpi.fail(BlackLib::BlackSPI::openErr) )
	{
	    std::cout << "OPENNING ERROR OCCURED" << std::endl;
	}
	else
	{
	    std::cout << "OPENNING IS OK" << std::endl;
	}
  while(1){
    uint8_t sendBytes[4] = { 0x87, 0x87, 0x87, 0x87 };
    uint8_t recvBytes[4];
    if(mySpi.transfer(sendBytes, recvBytes, sizeof(sendBytes), 100)){
      std::cout << "got: " << (char)recvBytes[0] << std::endl;
    }
    
  }

	return 0;
}
