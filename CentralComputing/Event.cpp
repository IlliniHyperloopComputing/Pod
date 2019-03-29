#include "Event.h"

void Event::wait() {
  std::unique_lock<std::mutex> lk(mutex);
  cond.wait(lk, [&]{ return condition; });
  lk.unlock();
}

void Event::wait_for(int64_t micros) {
  std::unique_lock<std::mutex> lk(mutex);
  cond.wait_for(lk, std::chrono::microseconds(micros), [&]{ return condition; });
  lk.unlock();
}

void Event::invoke() {
  std::unique_lock<std::mutex> lk(mutex);
  condition = true;
  lk.unlock();
  cond.notify_all(); 
}

void Event::reset() {
  std::unique_lock<std::mutex> lk(mutex);
  condition = false;
  lk.unlock();
}
