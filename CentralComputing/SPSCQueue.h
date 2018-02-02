#ifndef SPSCQUEUE_H
#define SPSCQUEUE_H

#include "Utils.h"
#include <queue>
#include <thread>
#include <mutex>

template <class T>
class SPSCQueue {
  
  public:
    /*
    * Constructs an SPSCQueue object
    */
    SPSCQueue();

    /*
    * Dequeues the object
    * @return the oldest object on the queue or nullptr if the queue is empty
    */
    T dequeue();

    /*
    * Enqueues an object
    * @param the object to enqueue
    */
    void enqueue(T object);

  private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
};

#endif //SPSCQUEUE_H
