#ifndef SAFEQUEUE_H_
#define SAFEQUEUE_H_

#include "Utils.h"
#include <queue>
#include <thread> // NOLINT
#include <mutex>  // NOLINT

template <class T>
class SafeQueue {  
 public:
  /*
  * Constructs an SafeQueue object
  */
  SafeQueue() {
    m_queue = std::queue<T>();
  }

  /*
  * Dequeues the object
  * @return the oldest object on the queue or nullptr if the queue is empty
  */
  
  bool dequeue(T * val) {
    std::lock_guard<std::mutex> guard(m_mutex);
    if (m_queue.empty()) {
      return false;
    } else {
      *val = m_queue.front();
      m_queue.pop();
      return true;
    }
  }

  /*
  * Enqueues an object
  * @param the object to enqueue
  */
  void enqueue(T object) {
    std::lock_guard<std::mutex> guard(m_mutex);
    m_queue.push(object);
  }

 private:
  std::queue<T> m_queue;
  std::mutex m_mutex;
};

#endif  // SAFEQUEUE_H_
