#ifndef DATA_H
#define DATA_H
#include <string.h>
#include <vector>

// Raw_Data objects exist inside the sensor's internal map and are updated during update_buffers

class Raw_Data {
  public:
      Raw_Data() {
        *this = Raw_Data(20);
      }
      Raw_Data(size_t size) : size(size) {
        raw = (uint8_t *) malloc(size);
      }

      Raw_Data(const Raw_Data& other) {
        copy(other);
      }

      void copy(const Raw_Data & other) {
        size = other.size;
        memcpy(raw, other.raw, size);
      }

      void clear() {
        free(raw);
      }
      ~Raw_Data(){
        clear();
      }
      
      Raw_Data & operator=(const Raw_Data& other){
        if (this != &other) {
          clear();
          copy(other);
        }
        return *this;
      }

      size_t size;
      uint8_t * raw;  
};

// Data are returned by get_data and are destroyed soon after
// They contain copies of raw_data
class Data { 
  public:
    ~Data() {
      free(raw);
    }    

    vector<double> actual;
    Raw_Data * raw;
};

#endif
