#include "Utils.h"
#include <chrono>
#include <unistd.h>
#include <errno.h>
void print_debug(const char * format, ...){
      #ifdef DEBUG
      va_list args;
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
      #endif
}

void print_info(const char * format, ...){
      va_list args;
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
}

void print_test(const char * format, ...){
      #ifdef TEST_POD
      va_list args;
      va_start(args, format);
      vfprintf(stderr, format, args);
      va_end(args);
      #endif
}

long long get_elapsed_time() {
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

ssize_t write_all_to_socket(int socket, uint8_t *buffer, size_t count) {
  size_t bytes_written = 0;
  while(bytes_written != count){
    //fprintf(stderr,"Writing to socket\n");
    int bytes = write(socket, buffer + bytes_written, count - bytes_written);
    if(bytes > 0)
      bytes_written += (size_t)bytes;
    else if(bytes == 0){
      fprintf(stderr, "Disconnected\n");
      return 0;
    } else if(bytes == -1 && errno != EINTR){
      fprintf(stderr, "Write failure!\n");
      return -1;
    }
  }
  return (ssize_t)bytes_written;
}
