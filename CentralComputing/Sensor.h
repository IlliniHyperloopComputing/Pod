#ifndef SENSOR_H
#define SENSOR_H

#include <map>
#include <vector>
#include <thread>
#include <mutex>

#include "Spi.h"

using namespace std;
enum Sensor_Type {
	THERMOCOUPLE,
	ACCELEROMETER,
	RIDE_HEIGHT,
	RETROREFLECTIVE,
	BRAKE_PRESSURE,
	TACHOMETER
};

static const int NUM_SENSORS = 6;

struct Sensor_Configuration {
	Sensor_Type type;
	int simulation; //simulation is an int that specifies the number of the test to be run
	size_t count;
	vector<int> addresses; //TODO: Figure out how to connect to each sensor


};

class Sensor_Group {

	public:
		/**
		* Constructs a sensor group
		* @param configuration the configuration for sensors
		**/
		Sensor_Group(Sensor_Configuration configuration);


		virtual ~Sensor_Group() = 0;
		
		// removed connect as per request by Richard
		/**
		* Connect to a sensor group on an XMEGA
		* @return connection status
		*
		virtual	bool connect() = 0;
		*/

		/**
		* Performs IO to get new sensor data from the XMEGA
		* OR uses a time function to set simulated values
		* @param a pointer to the spi object from which data will be extracted
		**/
		virtual void update(const Spi* spi) = 0;


		/**
		* Recalibrates and resets all sensors in the group.
		* May not do anything for some sensors
		**/
		virtual void reset() = 0;

		/**
		* Returns all available sensor data
		**/
		vector<double> get_data();

		mutex sensor_group_mutex;



	protected:
		
		int simulation;

		Sensor_Type type;

		vector<double> data;
		
		uint8_t device = 0;
		int idx = 0;
};

class Thermocouple : public Sensor_Group {

	public:
		Thermocouple(Sensor_Configuration configuration);

		~Thermocouple();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(const Spi* spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
		
		uint8_t device = 0;
		int idx = 0;
};

#endif
