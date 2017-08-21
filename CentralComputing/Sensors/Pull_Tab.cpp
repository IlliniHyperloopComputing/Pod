#include "../Sensor_Package.h"
#include <iostream>

using namespace std;

Pull_Tab::Pull_Tab(Sensor_Configuration configuration) : Sensor_Group(configuration) {
	count = 1;
	data = vector<double>(count);	
	
	translation_array = {{NO_TRANS }};
	name = "Pull_Tab";
	name_array = {{"Tab"}};
}

Pull_Tab::~Pull_Tab(){
	//do nothing
}

void Pull_Tab::reset() {
	//TODO: implement resetting
}

void Pull_Tab::update(Spi * spi) {
  (void) spi;

	switch(simulation) {
		case 0:
    {    
      int fd = open("/sys/class/gpio/gpio60/value", O_RDONLY);
      if(fd > 0){
        char buf = 'z'; 
        read(fd, &buf, 1);
        close(fd);
        if(buf == '1'){
          data[0] = 1;
        }
        else if(buf == '0'){
          data[0] = 0;
        }
      }
      else{
        perror("Error opening gpio60 value pin: ");
      }
    }
			break;
		case 1:
			simulation_1();
			break;
	}
}



void Pull_Tab::simulation_1() {
	auto start = Sensor_Package::start_time;
	auto now = Sensor_Package::get_current_time();
	auto difference = now - start;
	
	//TODO calc some actual value
	
	sensor_group_mutex.lock();
	for(size_t i = 0; i < data.size(); i++) {
		data[i] = i;
	}
	sensor_group_mutex.unlock();
}

uint8_t *  Pull_Tab::get_data_buffer() {
	uint8_t * buffer = (uint8_t * )malloc(get_buffer_size());

	sensor_group_mutex.lock();
	buffer[0] = data[0];	
	sensor_group_mutex.unlock();

	return buffer;
}

size_t Pull_Tab::get_buffer_size() {
	// 1 * uint16_t
	return 1 * sizeof(uint8_t);
}
