#include "Sensor.h"
#include <iostream>
#include <unistd.h>


using namespace std;

int main() {
	//set up configuration
	Sensor_Configuration c;
	c.type = THERMOCOUPLE;
	c.simulation = 1;
	c.count = 4;

	vector<Sensor_Configuration> configs;
	configs.push_back(c);

	Sensor_Package p(configs);
	usleep(200000);
	p.update();
	
	cout << "Mostly works!" << endl;
}
