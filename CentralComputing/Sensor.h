#include <map>
#include <vector>

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

		//Each sensor group instance also has a mutex that is grabbed on calling any function

		/**
		* Constructs a sensor group
		* @param configuration the configuration for sensors
		**/
		Sensor_Group(Sensor_Configuration configuration);


		virtual ~Sensor_Group() = 0;

		/**
		* Connect to a sensor group on an XMEGA
		* @return connection status
		**/
		virtual	bool connect() = 0;

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

	protected:
	
		int simulation;

		Sensor_Type type;

		vector<double> data;

		vector<int> addresses; //TODO figure out how each sensor group connects to the XMEGA

};

class Thermocouple : public Sensor_Group {

	public:
		Thermocouple(Sensor_Configuration configuration);

		~Thermocouple();
		/**
		* Connects to XMEGA
		**/
		bool connect();

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


class Sensor_Package {
	public:	
		/**
		* Creates a SensorPackage
		* @param  configuration a vector of sensor configurations for each sensor.  Initalized in the main thread
		**/
		Sensor_Package(vector<Sensor_Configuration> configuration);

		/**
		* Destructor, cleans up map and deletes sensor_group objects
		**/
		~Sensor_Package();

		/**
		* Calls update on every sensor group
		**/
		void update();

		/**
		* Sends a reset command to each sensor group
		**/
		void reset(); 

		/**
		* Returns a vector of the sensor data from a sensor group
		* @return a vector of sensor data
		**/
		vector<double> get_sensor_data(Sensor_Type type);

		static long long get_current_time();


		static long long start_time;
	private:
		map<Sensor_Type, Sensor_Group * > sensor_groups;

};
