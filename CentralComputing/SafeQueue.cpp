#include "SafeQueue.h"

template <class T>
SPSCQueue<T>::SPSCQueue() {
  //nothing to do here
}

template <class T>
T SPSCQueue<T>::dequeue() {
  std::lock_guard<std::mutex> guard(m_mutex);
  if(m_queue.empty()){
    return nullptr;
  } else {
    T ret = m_queue.front();
    m_queue.pop();
    return ret;
  }
}

template <class T>
void SPSCQueue<T>::enqueue(T object) {
  std::lock_guard<std::mutex> guard(m_mutex);
  m_queue.push(object);
}
