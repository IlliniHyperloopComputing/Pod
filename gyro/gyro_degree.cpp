// run apt-get install libi2c-dev  OR this code won't compile (probably)
#include <linux/i2c-dev.h> //https://xgoat.com/wp/2007/11/11/using-i2c-from-userspace-in-linux/

#include <sys/stat.h> //http://linux.die.net/man/3/open
#include <fcntl.h> //this and <sys/stat.h> are used for opening the i2c adapter
#include <errno.h> //Because opening files can error

#include <sys/ioctl.h>

#include <iostream>
#include <stdint.h>

#define GYRO_ADDRESS0 0x6B
#define GYRO_ADDRESS1 0x6A

using namespace std;
int main(){
	int i2c_1;
	int i2c_2;
	i2c_1 = open("/dev/i2c-1", O_RDWR); 
	i2c_2 = open("/dev/i2c-1", O_RDWR); 
	
	if(i2c_1 < 0){
		cout << "Something went wrong opening the i2c-1" <<endl;
		cout << "Error code: " << errno << endl;
		return 2;
	}
	if(i2c_2 < 0){
		cout << "Something went wrong opening the i2c-2" <<endl;
		cout << "Error code: " << errno << endl;
		return 2;
	}

	if( ioctl( i2c_1, I2C_SLAVE, GYRO_ADDRESS0) < 0){
		cout << "Failed to set GYRO0 slave address" <<endl;
		cout << "Error Code: " << cerr << endl;
		return 2;
	}
	if( ioctl( i2c_2, I2C_SLAVE, GYRO_ADDRESS1) < 0){
		cout << "Failed to set GYRO1 slave address" <<endl;
		cout << "Error Code: " << cerr << endl;
		return 2;
	}

	//Everything should be working, so lets transmit some data
	
	i2c_smbus_write_byte_data(i2c_1, 0x20, 0x0F); //set normal mode
	i2c_smbus_write_byte_data(i2c_1, 0x23, 0x00); //set resolution to 245 dps
	//i2c_smbus_write_byte_data(i2c_1, 0x24, 0x40); //turn on FIFO
	//i2c_smbus_write_byte_data(i2c_1, 0x2E, 0x2F); //set fifo
		////set fifo_ctrl to dynamic stream mode with fth to 15
	
	//set fifo_ctrl to 11000100 -> dynamic stream mode, and seting fth to 4
	//then need to check fifo_src(fth) if we have at least fth to read
	//then read fifo_src(fss4:0)+1

	//i2c_smbus_write_byte_data(i2c_2, 0x20, 0x0F);//set normal mode
	//i2c_smbus_write_byte_data(i2c_2, 0x23, 0x00); //set resolution
	
	double xpos = 0;
	double ypos = 0;
	double zpos = 0;
	uint8_t xhi0 =0; 
	uint8_t xlo0 =0;
	uint8_t yhi0 =0;
	uint8_t ylo0 =0;
	uint8_t zhi0 =0;
	uint8_t zlo0 =0;
	int16_t xdata0 = 0;
        int16_t ydata0 =0;
        int16_t zdata0 =0;
	uint8_t fth = 0;

	uint8_t * buff = new uint8_t[40];
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
			
	
			xpos += xdata0 *0.01;
			ypos += ydata0 *0.01;
			zpos += zdata0 *0.01;


			cout << "x: "<< xpos<< "\ty: " <<ypos<< "\tz: " <<zpos<<endl; //"\t"<< xdata1<< endl;
			//cout << "x: "<< xdata0<< "\ty: " <<ydata0 << "\tz: " <<zdata0<<endl; //"\t"<< xdata1<< endl;
		}
//		fth = i2c_smbus_read_byte_data(i2c_1, 0x2F);
//		cout << "fth value" << int(fth)<<endl;
//		if((fth & 0x80) == 0x80){//if there are enough data to read
//			for(uint8_t i = fth & 16; i>0; i--){//get the level of data samples to read
//				xhi0 = i2c_smbus_read_byte_data(i2c_1, 0x29);
//				xlo0 = i2c_smbus_read_byte_data(i2c_1, 0x28);
//				yhi0 = i2c_smbus_read_byte_data(i2c_1, 0x2B);
//				ylo0 = i2c_smbus_read_byte_data(i2c_1, 0x2A);
//				zhi0 = i2c_smbus_read_byte_data(i2c_1, 0x2D);
//				zlo0 = i2c_smbus_read_byte_data(i2c_1, 0x2C);
//				
//				int16_t xdata0 = (int16_t)(xlo0 | (xhi0 << 8));
//				int16_t ydata0 = (int16_t)(ylo0 | (yhi0 << 8));
//				int16_t zdata0 = (int16_t)(zlo0 | (zhi0 << 8));
//				
//				xdata0 *= 0.00875;
//				ydata0 *= 0.00875;
//				zdata0 *= 0.00875;	
//
//				cout << "x: "<< xdata0<< "\ty: " <<ydata0 << "\tz: " <<zdata0<<endl; //"\t"<< xdata1<< endl;
//				
//			}
			
				
			//int bytes_read = i2c_smbus_read_block_data(i2c_1,0x28,buff);
			//read_block_data causes kernal panic! Yay!

			//cout << "read_bytes: "<<bytes_read<<endl;
			//for(int j = 0; j< bytes_read; j++){
			//	cout << int(buff[j]) <<endl;
			//}
//		}
		
	//	uint8_t xhi0 = i2c_smbus_read_byte_data(i2c_1, 0x29);
	//	uint8_t xlo0 = i2c_smbus_read_byte_data(i2c_1, 0x28);
	//	uint8_t yhi0 = i2c_smbus_read_byte_data(i2c_1, 0x2B);
	//	uint8_t ylo0 = i2c_smbus_read_byte_data(i2c_1, 0x2A);
	//	uint8_t zhi0 = i2c_smbus_read_byte_data(i2c_1, 0x2D);
	//	uint8_t zlo0 = i2c_smbus_read_byte_data(i2c_1, 0x2C);
	//	
	//	int16_t xdata0 = (int16_t)(xlo0 | (xhi0 << 8));
	//	int16_t ydata0 = (int16_t)(ylo0 | (yhi0 << 8));
	//	int16_t zdata0 = (int16_t)(zlo0 | (zhi0 << 8));
	//	
	//	xdata0 *= 0.00875;
	//	ydata0 *= 0.00875;
	//	zdata0 *= 0.00875;	
	//	
	//	//float xfloat_data0 = xdata0 * 0.00875;
	//	cout << "x: "<< xdata0<< "\ty: " <<ydata0 << "\tz: " <<zdata0<<endl; //"\t"<< xdata1<< endl;
		
		//for(int j = 0; j <100; j++){};//the best way to "wait"
	}
	delete buff;
}
