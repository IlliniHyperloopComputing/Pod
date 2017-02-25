
#include "BlackLib/v3_0/BlackCore.h"
#include "BlackLib/v3_0/BlackSPI/BlackSPI.h"


int main(){
	BlackLib::BlackSPI  mySpi(BlackLib::SPI0_0, 8, BlackLib::SpiDefault, 2400000);
	if( mySpi.fail(BlackLib::BlackSPI::dtErr) or mySpi.fail(BlackLib::BlackSPI::portPathErr) )
	{
	    std::cout << "BlackSPI INITIALIZATION FAILED" << std::endl;
	}
	else
	{
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
	uint8_t sendBytes[4] = { 0x87, 0x41, 0xF1, 0x5A };
	uint8_t recvBytes[4];
	mySpi.transfer(sendBytes, recvBytes, sizeof(sendBytes), 100);
	if( mySpi.fail(BlackLib::BlackSPI::transferErr) )
	{
	    if( mySpi.fail(BlackLib::BlackSPI::openErr) )
	    {
		std::cout << "TRANSFER ERROR OCCURED BECAUSE SPI TTY IS NOT OPEN" << std::endl;
	    }
	    else
	    {
		std::cout << "TRANSFER ERROR OCCURED" << std::endl;
	    }
	}
	else
	{
	    std::cout << "TRANSFER IS OK" << std::endl;
	}

	return 0;
}
