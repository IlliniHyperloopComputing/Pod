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
#include "Command.h"

namespace Utils {

  const int HEARTBEAT_GPIO = 37;

  /**
  * Gets the current time since program startup
  * @return a long long representing the number of microseconds since startup
  **/
  int64_t microseconds();

  // To make error flag setting easy, this helper functionn is defined
  // Why use this instead of just calling Command::put() ? 
  // This function is "safe" and will not spam the queue.
  // Say something is faulting every cycle. Initially it will set the error flag
  // But then it send another command to the Unified Queue once every second. 
  // This helper function, and the int64_t array of timers makes that possible
  extern int64_t error_flag_timers[8*6];  // 8 flags per error ID, 6 errors
  void set_error_flag(uint8_t id, uint8_t value);

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
