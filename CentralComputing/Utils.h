#ifndef UTILS_H_
#define UTILS_H_
#include <cmath>
#include <algorithm>
#include <chrono> // NOLINT
#include <unistd.h>
#include <errno.h>
#include <string>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

namespace Utils {

  const int HEARTBEAT_GPIO = 37;

  /**
  * Gets the current time since program startup
  * @return a long long representing the number of microseconds since startup
  **/
  int64_t microseconds();

  // Set GPIO to specific value
  bool set_GPIO(int GPIONumber, bool switchVal);

  void busyWait(int64_t microseconds);
  enum LogLevel {
    LOG_EDEBUG = 0,  // excesive debug
    LOG_DEBUG = 1,   // debug
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
      print(LogLevel::LOG_ERROR, "%s errno: %s\n", S, err_str);\
    }

  /**
  * Define utility clamp function
  **/ 
  template <class T>
  T clamp(T v, T l, T h) {
    if (v < l) {
      return l;
    } else if (v > h) {
      return h;
    } else {
      return v;
    }
  }

  ssize_t write_all_to_socket(int socket, uint8_t * buffer, size_t count);

}  // namespace Utils

#endif  // UTILS_H_
