/////////////////////
////////////////////
//This is litterally just a copy of gyro_degree.cpp with a few changes for sending data
////////////////////
/////////////////////

#include <linux/i2c-dev.h> //https://xgoat.com/wp/2007/11/11/using-i2c-from-userspace-in-linux/

#include <sys/stat.h> //http://linux.die.net/man/3/open
#include <fcntl.h> //this and <sys/stat.h> are used for opening the i2c adapter
#include <errno.h> //Because opening files can error

#include <sys/ioctl.h>

#include <iostream>
#include <stdint.h>

#include "../tcp/client_tcp.h"

#define GYRO_ADDRESS0 0x6B
#define GYRO_ADDRESS1 0x6A

using namespace std;

int open_i2c_port(int address){
	int i2c = open("/dev/i2c-1", O_RDWR);
	
	if(i2c < 0){
		cout << "Something went wrong opening the i2c port" <<endl;
		cout << "Error code: " << errno << endl;
		return 2;
	}

	if( ioctl( i2c, I2C_SLAVE, address) < 0){
		cout << "Failed to set i2c (address: " << address << ") slave address" <<endl;
		cout << "Error Code: " << cerr << endl;
		return 2;
	}
	
	return i2c;
}

int main(){
	//////////////////////////////
	//Relavant additions
	double att_data[] = {0.0,0.0,0.0};	
	char *ip = "172.16.233.43";
	char *port = "4973";
	client_tcp c(ip, port);
	/////////////////////////////////
		
	
	int i2c_1 = open_i2c_port(GYRO_ADDRESS0);
	int i2c_2 = open_i2c_port(GYRO_ADDRESS1);
	//Everything should be working, so lets transmit some data
	
	i2c_smbus_write_byte_data(i2c_1, 0x20, 0x0F); //set normal mode (enable x,y,z axis) (set Hz to 100)
	i2c_smbus_write_byte_data(i2c_1, 0x23, 0x00); //set resolution to 245 dps (sensitiity = 0.00875)
	
	double xpos = 0;//The roll/pitch/yaw values
	double ypos = 0;
	double zpos = 0;

	uint8_t xhi0 =0; //used when reading in data
	uint8_t xlo0 =0;
	uint8_t yhi0 =0;
	uint8_t ylo0 =0;
	uint8_t zhi0 =0;
	uint8_t zlo0 =0;
	int16_t xdata0 = 0;
        int16_t ydata0 =0;
        int16_t zdata0 =0;
	uint8_t fth = 0;

	for(int i =0; i<10000; i++){
		fth = i2c_smbus_read_byte_data(i2c_1, 0x27);
		if(fth & 8 == 8){
			xhi0 = i2c_smbus_read_byte_data(i2c_1, 0x29);  			
                        xlo0 = i2c_smbus_read_byte_data(i2c_1, 0x28);
                        yhi0 = i2c_smbus_read_byte_data(i2c_1, 0x2B);
                        ylo0 = i2c_smbus_read_byte_data(i2c_1, 0x2A);
                        zhi0 = i2c_smbus_read_byte_data(i2c_1, 0x2D);
                        zlo0 = i2c_smbus_read_byte_data(i2c_1, 0x2C);
                        
                        xdata0 = (int16_t)(xlo0 | (xhi0 << 8));
                        ydata0 = (int16_t)(ylo0 | (yhi0 << 8));
                        zdata0 = (int16_t)(zlo0 | (zhi0 << 8));

			xdata0 *= 0.00875;// multiplying by resolution
			ydata0 *= 0.00875;
			zdata0 *= 0.00875;	
	
			/////////////////////////////////////////
			att_data[0] += xdata0 *0.01;// multiplying by frequency (default is 100 Hz)
			att_data[1] += ydata0 *0.01;
			att_data[2] += zdata0 *0.01;
			
			c.send_ATT(att_data);
			c.read();
			/////////////////////////////////////////
		}
	}

	cout << "x: "<< xpos<< "\ty: " <<ypos<< "\tz: " <<zpos<<endl; //"\t"<< xdata1<< endl;
}
