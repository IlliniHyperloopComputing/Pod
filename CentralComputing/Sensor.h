#ifndef SENSOR_H
#define SENSOR_H

#include <map>
#include <vector>
#include <thread>
#include <mutex>

#include "Spi.h"

#define XMEGA1 0
#define XMEGA2 1

#define MAX_SENSORS 30
#define ADC_TRANS (1.0/32768.0 * 4.096)


using namespace std;
enum Sensor_Type {
	THERMOCOUPLE,
	ACCELEROMETERX,
	ACCELEROMETERYZ,
	RIDE_HEIGHT,
	TAPE_COUNT,
	OPTICAL,
	BRAKE_PRESSURE,
	BATTERY,
  	CURRENT,
	PULL_TAB,
	TRUE_POSITION,
	TRUE_VELOCITY,
	TRUE_ACCELERATION
};

enum Sensor_Index_1 {
	X_ACCELERATION_INDEX = 0,
	BRAKE_PRESSURE_INDEX = 3,
	OPTICAL_INDEX = 4 
};

enum Sensor_Index_2 {
  YZ_ACCELERATION_INDEX = 0,
	RIDE_HEIGHT_INDEX = 2,
	BATTERY_CELL_INDEX = 5,
	THERMOCOUPLE_INDEX = 7,
  CURRENT_INDEX = 12,
	TAPE_COUNT_INDEX = 13
};



static const int NUM_SENSORS = 10;

struct Sensor_Configuration {
	Sensor_Type type;
	int simulation; //simulation is an int that specifies the number of the test to be run


};

class Sensor_Group {

	public:
		/**
		* Constructs a sensor group
		* @param configuration the configuration for sensors
		**/
		Sensor_Group(Sensor_Configuration configuration);


		virtual ~Sensor_Group() = 0;
		
		/**
		* Virtual function implemented by child classes 
		* Updates the values from the SPI buffers 
		* OR uses a time function to set simulated values
		**/
		virtual void update(Spi * spi) = 0;

		/**
		* Recalibrates and resets all sensors in the group.
		* May not do anything for some sensors
		**/
		virtual void reset() = 0;

		/**
		* Gets all data and stores it into a buffer
		**/
		virtual uint8_t * get_data_buffer() = 0;

		/**
		* Calculates the size of the buffer for each sensor
		**/
		virtual size_t get_buffer_size() = 0;

		/**
		* Returns all available sensor data
		**/
		virtual vector<double> get_data();

		/** 
		* Helper function
		* Refreshes the local data array from the spi buffers
		**/
		void refresh_data(Spi * spi);	

		/**
		* Prints data to standard out
		* Used for debugging only
		**/
		void print_data();



	protected:
	
		const int simulation;

		Sensor_Type type;

		vector<double> data;

		mutex sensor_group_mutex;
		size_t first_index = 0; // index offset to read from spi
		size_t device = 0; //xmega device number (0 or 1)
		size_t count = 0; //number of sensors
    	array<double, MAX_SENSORS> translation_array = {{0}};
    	string name = "Sensor Group";
    	array<string, MAX_SENSORS> name_array = {{"Sensor Group"}};

};

class Thermocouple : public Sensor_Group {

	public:
		Thermocouple(Sensor_Configuration configuration);

		~Thermocouple();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();

		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();



	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class XAccelerometer : public Sensor_Group {

	public:
		XAccelerometer(Sensor_Configuration configuration);

		~XAccelerometer();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();


	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class YZAccelerometer : public Sensor_Group {

	public:
		YZAccelerometer(Sensor_Configuration configuration);

		~YZAccelerometer();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();


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
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();

		

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
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();


	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Optical : public Sensor_Group {

	public:
		Optical(Sensor_Configuration configuration);

		~Optical();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();



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
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();

	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};


class Battery : public Sensor_Group {

	public:
		Battery(Sensor_Configuration configuration);

		~Battery();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();



	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Current : public Sensor_Group {

	public:
		Current(Sensor_Configuration configuration);

		~Current();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();




	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Pull_Tab : public Sensor_Group {

	public:
		Pull_Tab(Sensor_Configuration configuration);

		~Pull_Tab();

		/**
		* Receives new data from the XMega or calls simulations
		**/
		void update(Spi * spi);

		/**
		* Resets and recalibrates sensors
		**/
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();


	private:
		/**
		* Simulates set values in the vector
		**/
		void simulation_1();
};

class Sensor_Package; 

class True_Sensor : public Sensor_Group {

	public:
		True_Sensor(Sensor_Configuration configuration, Sensor_Package * pack);
		~True_Sensor();

		void update(Spi * spi);
		void reset();
		/**
		* Gets all data and stores it into a buffer
		**/
		uint8_t * get_data_buffer();

		/**
		* Calculates the size of the buffer for each sensor
		**/
		size_t get_buffer_size();
	
	protected:
		Sensor_Package * package;
};


class True_Position : public True_Sensor {
	public:	
		True_Position(Sensor_Configuration configuration, Sensor_Package * pack);
		void update(Spi * spi);
};



class True_Acceleration : public True_Sensor {

	public:	
		True_Acceleration(Sensor_Configuration configuration, Sensor_Package * pack);
		void update(Spi * spi);
};

class True_Velocity : public True_Sensor {

	public:	
		True_Velocity(Sensor_Configuration configuration, Sensor_Package * pack);
		void update(Spi * spi);
};


#endif
