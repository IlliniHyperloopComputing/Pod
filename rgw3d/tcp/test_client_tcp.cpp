#include "client_tcp.h"

double data[] = {0.0,0.1,-0.1,20,0.1,-0.1,10,0,0,5,2,1,21,50,45,100,90,80};
double vel[] = {0.0,0.1,-0.1,20,0.1,-0.1,10,0,0};
double data_size = 20;

string makeDataString(){	
	string result = "";
	for(int i = 0; i < data_size; i++){
		char tmp[21];
		sprintf(tmp,"%020F", data[i]);
		result = result + tmp;
	} 
	size_t size = result.length();
	char tmp[5];
	sprintf(tmp,"%04d", size);
	result = tmp + result;
	return result;
}


void updateData(){
	double ran =(rand()%10-5)/10.0;

	data[0] += 10;
	data[1] += ran;
	data[2] += ran;
	data[3] += 1;
	data[4] += ran;
	data[5] += ran;
	data[6] += .1;
	for(int i = 7; i<20; i++){
		data[i] += ran;
	}
}
void updateDataVel(){
	double ran =(rand()%10-5)/10.0;

	data[0] += 10;
	data[1] += ran;
	data[2] += ran;
	data[3] += 1;
	data[4] += ran;
	data[5] += ran;
	data[6] += .1;
}

int main(int argc, char* argv[]){
	client_tcp c(argv[1],argv[2]);
	for(int i=0; i< 200; i++){
		c.send_KIN(vel);
		updateDataVel();
		std::cout << "Reply is: ";
		std::cout << c.read();
		std::cout << std::endl;
	}
	


}
