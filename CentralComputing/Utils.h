#ifndef UTILS_H
#define UTILS_H
#include <cmath>
#include <algorithm>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

namespace Utils {
/**
* Gets the current time since program startup
* @return a long long representing the number of microseconds since startup
**/
long long microseconds();

enum LogLevel {
  LOG_EDEBUG = 0, //excesive debug
  LOG_DEBUG = 1, //debug
  LOG_INFO = 2,
  LOG_ERROR = 3
};

extern LogLevel loglevel;

void print(LogLevel level, const char * format, ...);

/**
* Prints errno, with user defined message prepended
**/
#define PRINT_ERRNO(S) \
  {\
    char err_buf[500];\
    char* err_str = strerror_r(errno, err_buf, (size_t)500);\
    print(LogLevel::LOG_ERROR, "%s errno: %s\n",S, err_str);\
  }

/**
* Define utility clamp function
**/ 
template <class T>
T clamp(T v, T l, T h) 
{
  if(v < l) return l;
  else if(v > h) return h; 
  else return v; 
}

ssize_t write_all_to_socket(int socket, uint8_t * buffer, size_t count);

}
#endif // UTILS_H

