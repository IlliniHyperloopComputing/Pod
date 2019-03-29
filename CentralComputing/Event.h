#ifndef EVENT_H_
#define EVENT_H_

#include <mutex> // NOLINT
#include <condition_variable> // NOLINT


// The Event object helps manage the pod's threads and is useful to wait for something to occur
// It's a pretty basic wrapper on a mutex/condition variable
// You can reuse an event by calling reset()
class Event {
 public:
  /*
   * Causes this thread to wait for another thread to invoke the event
   */
  void wait();

  /*
   * Causes this thread to wait for a time period or until the event is invoked
   */
  void wait_for(int64_t micros);

  /*
   * Wakes up all waiting threads
   */
  void invoke();  

  /*
   * Reset Event
   */
  void reset();

 private:
  std::mutex mutex;
  std::condition_variable cond;
  bool condition = false;
};
#endif  // EVENT_H_
