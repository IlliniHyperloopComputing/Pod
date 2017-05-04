#ifndef SENSOR_H
#define SENSOR_H

#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include "Spi.h"

#define XMEGA1 0
#define XMEGA2 1


using namespace std;
enum Sensor_Type {
	THERMOCOUPLE,
	ACCELEROMETER,
	RIDE_HEIGHT,
	TAPE_COUNT,
	POSITION,
	BRAKE_PRESSURE,
	PRIMARY_BATTERY,
	SECONDARY_BATTERY
};

enum Sensor_Index_1 {
	X_ACCELERATION_INDEX = 0,
	POSITION_INDEX = 1,
	BRAKE_PRESSURE_INDEX = 2
};

enum Sensor_Index_2 {
	PRIMARY_BATTERY_CELL_INDEX = 0,
	SECONDARY_BATTERY_CELL_INDEX = 4,
	TAPE_COUNT_INDEX = 8,
	THERMOCOUPLE_INDEX = 9,
	RIDE_HEIGHT_INDEX = 13
};



static const int NUM_SENSORS = 6;

struct Sensor_Configuration {
	Sensor_Type type;
	int simulation; //simulation is an int that specifies the number of the test to be run
	size_t count; //number of sensors
	size_t first_index; // index offset to read from spi
	size_t device; //xmega device number (0 or 1)


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
		**/
		virtual void update() = 0;


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


};

class Thermocouple : public Sensor_Group {

	public:
		Thermocouple(Sensor_Configuration configuration);

		~Thermocouple();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Accelerometer : public Sensor_Group {

	public:
		Accelerometer(Sensor_Configuration configuration);

		~Accelerometer();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Ride_Height : public Sensor_Group {

	public:
		Ride_Height(Sensor_Configuration configuration);

		~Ride_Height();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};


class Tape_Count : public Sensor_Group {

	public:
		Tape_Count(Sensor_Configuration configuration);

		~Tape_Count();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Position : public Sensor_Group {

	public:
		Position(Sensor_Configuration configuration);

		~Position();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Brake_Pressure : public Sensor_Group {

	public:
		Brake_Pressure(Sensor_Configuration configuration);

		~Brake_Pressure();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};


class Primary_Battery : public Sensor_Group {

	public:
		Primary_Battery(Sensor_Configuration configuration);

		~Primary_Battery();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Secondary_Battery : public Sensor_Group {

	public:
		Secondary_Battery(Sensor_Configuration configuration);

		~Secondary_Battery();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update();

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

#endif
