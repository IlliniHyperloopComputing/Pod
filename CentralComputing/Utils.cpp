#include "Utils.h"
#include <chrono>
#include <unistd.h>
#include <errno.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

Utils::LogLevel Utils::loglevel = LOG_ERROR;

void Utils::print(LogLevel level, const char * format, ...){
      if(loglevel <= level){
        char buffer[256];
        if(level == LOG_ERROR){
          snprintf(buffer, 256, "%s[ERROR]:%s %s", ANSI_COLOR_RED, ANSI_COLOR_RESET, format);
        }
        else if(level == LOG_INFO){
          snprintf(buffer, 256, "%s[INFO ]:%s %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, format);
        }
        else if(level == LOG_DEBUG){
          snprintf(buffer, 256, "%s[DEBUG]:%s %s", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, format);
        }
        else if(level == LOG_EDEBUG){
          snprintf(buffer, 256, "%s", format);
        }

        va_list args;
        va_start(args, format);
        vfprintf(stdout, buffer, args);
        va_end(args);
      }
}

long long Utils::microseconds() {
  static long long start_time = -1;
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  if(start_time == -1){
    start_time = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return 0;
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count() - start_time;
  }
}

ssize_t Utils::write_all_to_socket(int socket, uint8_t *buffer, size_t count) {
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

