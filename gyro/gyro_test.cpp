// run apt-get install libi2c-dev  OR this code won't compile (probably)
#include <linux/i2c-dev.h> //https://xgoat.com/wp/2007/11/11/using-i2c-from-userspace-in-linux/

#include <sys/stat.h> //http://linux.die.net/man/3/open
#include <fcntl.h> //this and <sys/stat.h> are used for opening the i2c adapter
#include <errno.h> //Because opening files can error

#include <sys/ioctl.h>

#include <iostream>
#include <stdint.h>

#define GYRO_ADDRESS 0x6B

using namespace std;
int main(){
	int i2c_1;
	i2c_1 = open("/dev/i2c-1", O_RDWR); 
	
	if(i2c_1 < 0){
		cout << "Something went wrong opening the i2c-1" <<endl;
		cout << "Error code: " << errno << endl;
		return 2;
	}

	if( ioctl( i2c_1, I2C_SLAVE, GYRO_ADDRESS) < 0){
		cout << "Failed to set GYRO slave address" <<endl;
		cout << "Error Code: " << cerr << endl;
		return 2;
	}

	//Everything should be working, so lets transmit some data
	
	i2c_smbus_write_byte_data(i2c_1, 0x20, 0x0F);//set normal mode
	i2c_smbus_write_byte_data(i2c_1, 0x23, 0x00); //set resolution
	
	cout << "yoo" <<endl;
	for(int i =0; i<10000; i++){
		uint8_t xhi = i2c_smbus_read_byte_data(i2c_1, 0x29);
		uint8_t xlo = i2c_smbus_read_byte_data(i2c_1, 0x28);
		
		int16_t xdata = (int16_t)(xlo | (xhi << 8));
		
		float xfloat_data = xdata * 0.00875;
		cout << xfloat_data << endl;
		
		for(int j = 0; j <100000; j++){};//the best way to "wait"
	}
}
