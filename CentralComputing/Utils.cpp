#include "Utils.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

Utils::LogLevel Utils::loglevel = LOG_ERROR;

void Utils::print(LogLevel level, const char * format, ...) {
  if (loglevel <= level) {
    char buffer[256];
    if (level == LOG_ERROR) {
      snprintf(buffer, sizeof(buffer), "%s[ERROR]:%s %s", ANSI_COLOR_RED, ANSI_COLOR_RESET, format);
    } else if (level == LOG_INFO) {
      snprintf(buffer, sizeof(buffer), "%s[INFO ]:%s %s", ANSI_COLOR_GREEN, ANSI_COLOR_RESET, format);
    } else if (level == LOG_DEBUG) {
      snprintf(buffer, sizeof(buffer), "%s[DEBUG]:%s %s", ANSI_COLOR_YELLOW, ANSI_COLOR_RESET, format);
    } else if (level == LOG_EDEBUG) {
      snprintf(buffer, sizeof(buffer), "%s", format);
    }

    va_list args;
    va_start(args, format);
    vfprintf(stdout, buffer, args);
    va_end(args);
  }
}

bool Utils::set_GPIO(int GPIONumber, bool switchVal) {
  std::string start = "/sys/class/gpio/gpio";
  std::string integer = std::to_string(GPIONumber);
  std::string end = "/value";
  std::string path = start + integer + end;
  std::ofstream out(path, std::ofstream::trunc);
  if (!out.is_open()) {
      return false;
  }
  if (switchVal == true) {
      out << "1";
  } else {
      out << "0";
  }
  out.close();
  return true;  // Have it return 1 if it works and zero otherwise
}

int64_t Utils::microseconds() {
  static int64_t start_time = -1;
  auto now = std::chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  if (start_time == -1) {
    start_time = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return 0;
  } else {
    return std::chrono::duration_cast<std::chrono::microseconds>(duration).count() - start_time;
  }
}

void Utils::busyWait(int64_t microseconds) {
  struct timespec currTime;
  clockid_t threadClockId;
  pthread_getcpuclockid(pthread_self(), &threadClockId);
  clock_gettime(threadClockId, &currTime);
  time_t secs = (microseconds / 1000000) + currTime.tv_sec + (currTime.tv_nsec / 1000000000);
  struct timespec toEnd;
  toEnd.tv_sec = secs;
  toEnd.tv_nsec = ((microseconds * 1000) + currTime.tv_nsec) % 1000000000;
  while (currTime.tv_sec <= toEnd.tv_sec) {
    if (currTime.tv_sec == toEnd.tv_sec && currTime.tv_nsec > toEnd.tv_nsec) {
      break;
    }
    clock_gettime(threadClockId, &currTime);
  }
  return;
}


ssize_t Utils::write_all_to_socket(int socket, uint8_t *buffer, size_t count) {
  size_t bytes_written = 0;
  while (bytes_written != count) {
    int bytes = write(socket, buffer + bytes_written, count - bytes_written);
    if (bytes > 0) {
      bytes_written += (size_t)bytes;
    } else if (bytes == 0) {
      fprintf(stderr, "Disconnected\n");
      return 0;
    } else if (bytes == -1 && errno != EINTR) {
      fprintf(stderr, "Write failure!\n");
      return -1;
    }
  }
  return (ssize_t)bytes_written;
}
