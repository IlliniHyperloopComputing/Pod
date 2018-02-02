#include "SafeQueue.h"

template <class T>
SafeQueue<T>::SafeQueue() {
  m_queue = std::queue<T>();
}

template <class T>
T SafeQueue<T>::dequeue() {
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
void SafeQueue<T>::enqueue(T object) {
  std::lock_guard<std::mutex> guard(m_mutex);
  m_queue.push(object);
}
