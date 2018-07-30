#include "Event.hpp"

void Event::wait() {
  std::unique_lock<std::mutex> lk(mutex);
  cond.wait(lk, [&]{ return condition; });
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
