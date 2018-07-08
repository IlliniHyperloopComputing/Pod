#ifndef SOURCE_MANAGER_BASE_HPP
#define SOURCE_MANAGER_BASE_HPP

#include "NetworkManager.hpp"
#include "Utils.h"

template <int Delay, class Data>
class SourceManagerBase {

  public:
    shared_ptr<Data> Get() {
      mutex.lock();
      shared_ptr<Data> ret = data;
      mutex.unlock();
      return ret;
    }

    void initialize() {
      data = refresh();
      running.store(true);
      worker = std::thread([&]() {
        refresh_loop(); // I don't know how to start a thread using a member function, but I know how to use lambdas so suck it C++
      });
    }
    
    void stop() {
      running.store(false);
      worker.join();
    }

  private:
    virtual shared_ptr<Data> refresh() = 0; //constructs a new Data object and fills it in

    void refresh_loop() {
      while(running.load()) {
        shared_ptr<Data> new_data = refresh();
        mutex.lock();
        data = new_data;
        mutex.unlock();
        usleep(Delay * 1e6);
      }
    }

    shared_ptr<Data> data;
    std::mutex mutex;
    std::atomic<bool> running;
    std::thread worker;
};
#endif
