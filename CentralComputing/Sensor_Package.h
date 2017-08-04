#ifndef SENSOR_PACKAGE_H
#define SENSOR_PACKAGE_H

#include <map>
#include <vector>
#include <thread>
#include <mutex>

#include "Sensor.h"
#include "Spi.h"

using namespace std;

class Sensor_Package {
	public:	
		/**
		* Creates a SensorPackage
		* @param  configuration a vector of sensor configurations for each sensor.  Initalized in the main thread
		**/
		Sensor_Package(vector<Sensor_Configuration> configuration, bool xmega_connect);

		/**
		* Destructor, cleans up map and deletes sensor_group objects
		**/
		~Sensor_Package();

		/**
		* Calls update on every sensor group
		* @param transfer the Xmega transfer struct
		**/
		void update(Xmega_Transfer & transfer);

		/**
		* Sends a reset command to each sensor group
		**/
		void reset(); 

		/**
		* Returns a vector of the sensor data from a sensor group
		* @return a vector of sensor data
		**/
		vector<double> get_sensor_data(Sensor_Type type);
    
    /**
		* Returns a pointer to the specified sensor group
		* @return a pointer to a sensor group
		**/
    Sensor_Group * get_sensor_group(Sensor_Type type);


		static long long get_current_time();
		static long long start_time;
		bool connect;
	private:
		map<Sensor_Type, Sensor_Group * > sensor_groups;
		Spi * spi;	

};

#endif
