#include "Utils.h"

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
