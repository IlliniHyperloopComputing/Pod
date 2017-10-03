#include "Utils.h"
#include <chrono>

void print_debug(const char * format, ...){
      #ifdef DEBUG
      va_list args;
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
      #endif
}

void print_test(const char * format, ...){
      #ifdef TEST_POD
      va_list args;
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
      #endif
}

long long get_current_time() {
  static long long start_time = -1;
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
  if(start_time == -1){
    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    return 0;
  } else {
	  return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() - start_time;
  }
}

